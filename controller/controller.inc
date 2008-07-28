<?php 
require_once("inc/db_conn.inc");
require_once("views/project.inc");
require_once("views/host.inc");
require_once("views/pool.inc");
require_once("model/data.inc");

class controller extends html
{
      public $db_user='ogm';
      public $db_password='ogm';
      public $db_host='localhost';
      public $db_name='ogm';
      public $db=''; // The DB handler
      public $auth=False;
      public $project;
      public $pool;
      public $language;
      public $host;


      public function __construct($language="es_ES.utf8")
      {
          
          session_start();
          //putenv("LANG=$language");
          //setlocale(LC_ALL, $language);
          //$domain = 'messages';
          //bindtextdomain($domain, "lang");
          //textdomain($domain);
          $this->db = new DbConn();
          $this->db->init_conn($this->db_user,$this->db_password,$this->db_host,$this->db_name);
          $this->language = $language;
          $this->pool = new HTMLpool($this->language,$_SESSION['role'],$_SESSION['supplier'],$this->db);
          $this->project = new HTMLproject($this->language,$_SESSION['role'],$this->db);
          $this->host = new HTMLhost($this->language,$_SESSION['role'],$this->db);

      }
      function authenticate($userid=null,$password=null)
      {
          if (($userid==null) or ($password==null))     
          {
              return(False);
          }
          else
          {
              $user =  $this->db->lookup_fields('user','user','id,role,supplier','id="'.$userid.'" and password="'.md5($password).'"');
              if (!$user==null)
              {
                $_SESSION['userid']=$user->id;
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

      function view($view='start',$role='default',$subaction=null,$msg=null)
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
                        $this->host->table($obj,$pools);
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

            case 'login':
            {
                $layout = new login_page($this->language);
                $layout->page();
                break;
            }
          }
      }

}
?>
