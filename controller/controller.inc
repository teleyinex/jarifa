<?php 
require_once("inc/db_conn.inc");
require_once("inc/project.inc");
require_once("model/data.inc");
require_once("views/class_view.inc");

class controller extends html
{
      public $db_user='ogm';
      public $db_password='ogm';
      public $db_host='localhost';
      public $db_name='ogm';
      public $db=''; // The DB handler
      public $auth=False;
      public $project;


      public function __construct()
      {
          
          session_start();
          $this->am = new accmng($this->db_user,$this->db_password,$this->db_host,$this->db_name);;
          $this->db = new DbConn();
          $this->db->init_conn($this->db_user,$this->db_password,$this->db_host,$this->db_name);

          $this->project = new HTMLproject;
          

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
                $this->auth=True;
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

      function project_list()
      {
          $obj = $this->db->enum_fields('project','project','*','1', 'order by id');
          return($obj);
      }

      function edit_project($values)
      {
          $obj = $this->db->lookup_fields('project','project','*','id="'.$values['id'].'"');
          return($obj);
      }

      function update_project($id,$share)
      {
        $obj = $this->db->lookup_id($id,'project','project');
        return($this->db->update($obj,'project','share = '.$share));
      }

      function delete_project($id)
      {
        $obj = $this->db->lookup_id($id,'project','project');
        return($this->db->delete($obj,'project'));
      }

      function insert_project($values)
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

            case 'project':
                {
                    switch ($subaction)
                    {
                        case 'edit':
                        {
                            $layout = new project_page($_SESSION['role'],'edit');
                            $layout->page();
                            $obj = $this->edit_project($_GET);
                            $this->project->edit($obj);
                            break;
                        }
                        case 'insert':
                        {
                            $layout = new project_page($_SESSION['role'],'insert');
                            $layout->page();
                            $this->project->insert();
                            break;
                        }
                        default:
                        {
                            $layout = new project_page($_SESSION['role'],'start');
                            $layout->page();
                            $obj = $this->project_list();
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
