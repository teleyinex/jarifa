<?php 
require_once("inc/db_conn.inc");
require_once("model/data.inc");
require_once("views/class_view.inc");

class controller
{
      public $db_user='ogm';
      public $db_password='ogm';
      public $db_host='localhost';
      public $db_name='ogm';
      public $db=''; // The DB handler
      public $auth=False;


      public function __construct()
      {
          
          session_start();
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
              $user =  $this->db->lookup_fields('users','user','id,role,supplier','id="'.$userid.'" and password="'.md5($password).'"');
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

      function view($view='start',$role='default',$msg=null)
      {
          if ($role=='default') $view='login';
          switch ($view)
          {

            case 'start':
            {
                switch ($role)
                {
                    case 'root':
                    {
                        $layout = new start_page('root');
                        $layout->page();
                        break;
                    }
                    case 'allocator':
                    {
                        $layout = new start_page('allocator');
                        $layout->page();
                        break;
                    }
                    case 'supplier':
                    {
                        $layout = new start_page('supplier');
                        $layout->page();
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
