<?php 
require_once("inc/db_conn.inc");
require_once("views/project.inc");
require_once("inc/pool.inc");
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


      public function __construct()
      {
          
          session_start();
          $this->db = new DbConn();
          $this->db->init_conn($this->db_user,$this->db_password,$this->db_host,$this->db_name);
          $this->project = new HTMLproject(null,$this->db);
          $this->pool = new HTMLpool(null,null,$this->db);

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

      //
      // Project Functions
      //

      //function project_list()
      //{
      //    $obj = $this->db->enum_fields('project','project','*','1', 'order by id');
      //    return($obj);
      //}

      //function edit_project($values)
      //{
      //    $obj = $this->db->lookup_fields('project','project','*','id="'.$values['id'].'"');
      //    return($obj);
      //}

      //function update_project($id,$share)
      //{
      //  $obj = $this->db->lookup_id($id,'project','project');
      //  return($this->db->update($obj,'project','share = '.$share));
      //}

      //function delete_project($id)
      //{
      //  $obj = $this->db->lookup_id($id,'project','project');
      //  return($this->db->delete($obj,'project'));
      //}

      //function insert_project($values)
      //{
      //    $insert=True;
      //    if (empty($values['name'])) $insert=False;
      //    if (empty($values['url'])) $insert=False;
      //    if (empty($values['signature'])) $insert=False;
      //    if (empty($values['authenticator'])) $insert=False;
      //    if ($insert)
      //    {
      //      $query = '(name,url,signature,authenticator,share) values ("'
      //        .$values['name'].'", "'.$values['url'].'","'.$values['signature'].'","'.$values['authenticator'].'",'.$values['share'].')';
      //      return($this->db->insert('project', $query));
      //    }
      //    else
      //        return(False);
      //}

      //
      // Pool Functions
      //

      function pool_list()
      {
          $obj = $this->db->enum_fields('pool','pool','*','1', 'order by id');
          return($obj);
      }

      function edit_pool($values)
      {
          $obj = $this->db->lookup_fields('pool','pool','*','id="'.$values['id'].'"');
          return($obj);
      }

      function update_pool($id,$name,$prefs)
      {
        $obj = $this->db->lookup_id($id,'pool','pool');
        return($this->db->update($obj,'pool','name = '.$name. ', $prefs='.$prefs));
      }

      function delete_pool($id)
      {
        $obj = $this->db->lookup_id($id,'pool','pool');
        return($this->db->delete($obj,'pool'));
      }

      function insert_pool($values)
      {
          $insert=True;
          if (empty($values['name'])) $insert=False;
          if (empty($values['url'])) $insert=False;
          if (empty($values['signature'])) $insert=False;
          if (empty($values['authenticator'])) $insert=False;
          if ($insert)
          {
            $query = '(name,url,signature,authenticator,share) values ("'
              .$values['name'].'", "'.$values['url'].'","'.$values['signature'].'","'.$values['authenticator'].'",'.$values['share'].')';
            return($this->db->insert('project', $query));
          }
          else
              return(False);
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

            case 'pool':
            {
                $this->pool->role = $_SESSION['role'];
                $this->pool->supplier = $_SESSION['supplier'];
                switch ($subaction)
                {
                    default:
                    {
                        $this->pool->page('start');
                        $obj = $this->pool_list();
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
                $layout = new login_page();
                $layout->page();
                break;
            }
          }
      }

}
?>
