<?php 
require_once("inc/html.inc");
require_once("views/class_view.inc");
require_once("controller/controller.inc");
require_once("inc/lang.inc");

$ctr = new controller();
// If the user has not been authenticated and there is not any action, the login screen must be shown
if (!isset($_SESSION['userid']) and (empty($_POST)))
{
    $ctr->view("login");
}

// If the user has not been authenticated and the action is to authenticate, the controller tries to validate the user. If 
// the user is a right one, it shows the start page, else it shows the login screen again.
if (!isset($_SESSION['userid']) and ($_POST['action']=='auth'))
    if ($ctr->authenticate($_POST['userid'],$_POST['password']))
        $_GET['action']='start';
    else
        $ctr->view("login");


// If the user has been authenticated, then the actions can be parsed:
if (isset($_SESSION['userid']))
{
    switch ($_GET['action'])
    {
        // Default screen
        case 'start':
        {
            $ctr->view("start",$_SESSION['role']);
            break;
        }

        // Projects action
        case 'project':
        {
            $ctr->view("project",$_SESSION['role']);
            break;
        }

        case 'ed_project':
        {
            $ctr->view("project",$_SESSION['role'],'edit');
            break;
        }

        case 'up_project':
        {
            if ($_POST['delete'])
                $ctr->delete_project($_GET['id']);
            else
                $ctr->update_project($_GET['id'],$_POST['share']);
            $ctr->view("project",$_SESSION['role']);
            break;
        }
        
        // Machines action
        case 'machine':
        {
            $ctr->view("machine",$_SESSION['role']);
            break;
        }
        
        // Pools action
        case 'pool':
        {
            $ctr->view("pool",$_SESSION['role']);
            break;
        }
        
        // Stats action
        case 'stat':
        {
            $ctr->view("stat",$_SESSION['role']);
            break;
        }


        // Logout action
        case 'logout':
        {
            $ctr->logout();
            break;        
        }

        // Default screen
        default:
        {
            $ctr->view("start",$_SESSION['role']);
            break;
        }
    }

}
?>
