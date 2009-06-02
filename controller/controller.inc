<?php 
/* 
Copyright 2008 Daniel Lombraña González, David P. Anderson, Francisco Fernández de Vega

This file is part of Jarifa.

Jarifa is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Jarifa is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with Jarifa.  If not, see <http://www.gnu.org/licenses/>.
*/

require_once("inc/db_conn.inc");
require_once("views/default.inc");
require_once("views/project.inc");
require_once("views/host.inc");
require_once("views/pool.inc");
require_once("views/user.inc");
require_once("views/stat.inc");
require_once("views/volunteer.inc");
require_once("views/ranking.inc");
require_once("views/map.inc");
require_once("model/data.inc");
//require_once("inc/lang.inc");

class controller extends html
{
      public $db_user;
      public $db_password;
      public $db_host;
      public $db_name;
      public $db=''; // The DB handler
      public $boinc_user_email;
      public $boinc_user_name;
      public $boinc_passwd;
      public $auth=False;
      public $project;
      public $pool;
      public $language;
      public $host;
      public $user;
      public $stat;
      public $context = False;
      public $proxy_name;
      public $proxy_port;
      public $proxy_request_fulluri;


      public function __construct()
      {
          
          session_start();
          $this->load_config();
          $this->db = new DbConn();
          $this->db->init_conn($this->db_user,$this->db_password,$this->db_host,$this->db_name);
          $this->pool = new HTMLpool($this->language,$_SESSION['role'],$_SESSION['supplier'],$this->db);
          $this->project = new HTMLproject($this->language,$_SESSION['role'],$this->db);
          $this->host = new HTMLhost($this->language,$_SESSION['role'],$_SESSION['supplier'],$this->db);
          $this->user = new HTMLuser($this->language,$_SESSION['role'],$this->db);
          $this->stat = new HTMLstat($this->language,$_SESSION['role'], $_SESSION['supplier'],$this->db);
          $this->volunteer = new HTMLvolunteer($this->language,"volunteer",$this->db);
          $this->ranking = new HTMLranking($this->language,"",$this->db);
          $this->map = new HTMLmap($this->language,"",$this->db);

      }

      function load_config()
      {
          $conf = simplexml_load_file("conf/jarifa.conf");
          $this->db_user = $conf->db->user;
          $this->db_password = $conf->db->password;
          $this->db_host = $conf->db->host;
          $this->db_name = $conf->db->name;
          $this->language = $conf->account_manager->language;
          $this->boinc_user_email = $conf->account_manager->boinc_user_email;
          $this->boinc_user_name = $conf->account_manager->boinc_user_name;
          $this->boinc_passwd = $conf->account_manager->boinc_passwd;
          // Are we using a proxy?
          if ($conf->xpath("/conf/proxy"))
          {
            error_log("Using proxy configuration");
            $this->proxy_name = $conf->proxy->name;
            $this->proxy_port = $conf->proxy->port;
            if ($conf->proxy->request_fulluri == 1) $this->proxy_request_fulluri = True;
            else $this->proxy_request_fulluri = False;
            $opts= array('http' => array('request_fulluri' => $this->proxy_request_fulluri,'proxy' =>'tcp://'.$this->proxy_name.':'.$this->proxy_port));
            $this->context = stream_context_create($opts);
          }
          else $this->context = null;

      }

      function create_boinc_user($url,$invitation=null)
      {
          // Web RPC for create a boinc account and return the weak authenticator
          $request='?email_addr='.$this->boinc_user_email.'&passwd_hash='.md5($this->boinc_passwd.$this->boinc_user_email).
                                                          '&user_name='.$this->boinc_user_name;
          if ($invitation != null) $request = $request . '&invite_code='.$invitation;
          $authenticator = file_get_contents(trim($url).'/create_account.php'.$request, false,$this->context);
          $xml = new SimpleXMLElement($authenticator);
         
          $weak_authenticator =
              file_get_contents(trim($url).'/am_get_info.php?account_key='.$xml->authenticator,false,$this->context);
          $xml = new SimpleXMLElement($weak_authenticator);
          return($xml->weak_auth);
      }

      function authenticate($userid=null,$password=null)
      {
          if (($userid==null) or ($password==null))     
          {
              return(False);
          }
          else
          {
              $user =  $this->db->lookup_fields('user','user','name,role,supplier','name="'.$userid.'" and password="'.md5($password).'"');
              if (!$user==null) 
              {
                $_SESSION['userid']=$user->name;
                $_SESSION['role']=$user->role;
                $_SESSION['supplier']=$user->supplier;
                return(True);
              }
              else
              {
                  $this->auth=False;
                  return(False);
              }
          }
      }


      function logout()
      {
          session_unset();
          session_destroy();
          header("Location: index.php");
      }

      function how_many_suppliers()
      {
          $user = $this->db->enum_fields('pool','pool','*','1','group by supplier');
          if (!$user == null) 
          {
             // The volunteer supplier has to be removed:
            return(count($user)-1);
          }
          else return(0);
      }


      function how_many_volunteers()
      {
          $user = $this->db->enum_fields('user','user','*','supplier="Volunteer"','');
          if (!$user == null) 
          {
            return(count($user));
          }
          else return(0);
      }

      function how_many_computers()
      {
          $user = $this->db->enum_fields('host','host','*','1','');
          if (!$user == null) 
          {
            return(count($user));
          }
          else return(0);
      }


      function view($view='start',$role='default',$subaction=null,$msg=null,$pos=null,$num=null,$init=null)
      {
          switch ($view)
          {

            case 'error':
            {
                $layout = new error_page($_SESSION['role'],$msg);
                $layout->page();
                break;
            }

            case 'start':
            {
                $layout = new start_page($_SESSION['role']);
                $layout->page();
                break;
            }

            case 'ranking':
            {
                $this->ranking->page("volunteers");
                break;
          
            }

            case 'map':
            {
                $this->map->page();
                break;
          
            }


            case 'volunteer':
            {
                //$this->volunteer->role = $_SESSION['role'];
                switch ($subaction)
                {
                    case 'thanks':
                    {
                        $this->volunteer->page('thanks');
                        break;
                    }

                    case 'in_volunteer':
                    {
                          if ($this->volunteer->insert($_POST,&$err_msg))
                            // If everything went OK, thank the user
                            $this->view("volunteer",null,"thanks");
                          else
                            // Else show an error page
                            $this->view("error",null,null,$err_msg);
                          break;
                    }

                    default:
                    {
                        $this->volunteer->page('start');
                        break;
                    }
                
                }
                break;
            }

            case 'stat':
            {
                $this->stat->role = $_SESSION['role'];
                switch ($subaction)
                {
                    case 'credit':
                    {
                        $this->stat->page('credit');
                        break;
                    }

                    case 'gflops':
                    {
                        $this->stat->page('gflops');
                        break;
                    }

                    default:
                    {
                        $this->stat->page('start');
                        break;
                    }
                
                }
                break;
            }


            case 'host':
            {
                $this->host->role = $_SESSION['role'];
                switch ($subaction)
                {
                    case 'edit':
                    {
                        $this->host->page('edit',$_GET);
                        break;
                    }

                    default:
                    {
                        $this->host->page('start');
                        $obj = $this->host->lists();
                        $pools = $this->host->pools();
                        $this->host->table($obj,$pools,$pos,$num,$init);
                        break;
                    }
                
                } 
                break;
            }

            case 'pool':
            {
                $this->pool->role = $_SESSION['role'];
                $this->pool->supplier = $_SESSION['supplier'];
                switch ($subaction)
                {
                    case 'edit':
                    {
                        $this->pool->page('edit',$_GET);
                        break;
                    }

                    case 'insert':
                    {
                        $this->pool->page('insert',$_POST);
                        break;
                    }

                    default:
                    {
                        $this->pool->page('start');
                        $obj = $this->pool->lists();
                        $this->pool->table($obj);
                        break;
                    }
                
                } 
                break;
            }

            case 'project':
                {
                    $this->project->role = $_SESSION['role'];
                    switch ($subaction)
                    {
                        case 'edit':
                        {
                            $this->project->page('edit',$_GET);
                            break;
                        }
                        case 'insert':
                        {
                            $this->project->page('insert');
                            break;
                        }
                        default:
                        {
                            $this->project->page('start');
                            $obj = $this->project->lists();
                            $this->project->table($obj);
                            break;
                        }

                    }
                    break;
                }

            case 'user':
                {
                    $this->user->role = $_SESSION['role'];
                    switch ($subaction)
                    {
                        case 'edit':
                        {
                            $this->user->page('edit',$_GET);
                            break;
                        }
                        case 'insert':
                        {
                            $this->user->page('insert');
                            break;
                        }
                        default:
                        {
                            $this->user->page('start');
                            $obj = $this->user->lists($_SESSION['userid']);
                            $this->user->table($obj);
                            break;
                        }

                    }
                    break;
                }

            case 'login':
            {
                $layout = new login_page($this->language);
                $layout->page($this->how_many_volunteers(),$this->how_many_computers(),$this->how_many_suppliers());
                break;
            }

          }
      }

}
?>
