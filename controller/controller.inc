<?php 
require_once("inc/db_conn.inc");
require_once("model/data.inc");
require_once("views/class_view.inc");
require_once("inc/acc.inc");

class controller
{
      public $db_user='ogm';
      public $db_password='ogm';
      public $db_host='localhost';
      public $db_name='ogm';
      public $db=''; // The DB handler
      public $auth=False;
      public $am;


      public function __construct()
      {
          
          session_start();
          $this->am = new accmng($this->db_user,$this->db_password,$this->db_host,$this->db_name);;
          $this->db = new DbConn();
          $this->db->init_conn($this->db_user,$this->db_password,$this->db_host,$this->db_name);
          

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
                            //$this->am->edit_project($_GET);
                            $layout = new project_page($_SESSION['role'],'edit');
                            $layout->page();
                            $obj = $this->am->edit_project($_GET);
                            $this->am->html->table_project_edit($obj);

                            break;
                        }
                        default:
                        {
                            $layout = new project_page($_SESSION['role'],'start');
                            $layout->page();
                            $obj = $this->am->project_list();
                            $this->am->html->table_project($obj);
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
