<?php 
require_once("inc/html.inc");
//require_once("inc/auth.inc");
require_once("views/class_view.inc");
require_once("controller/controller.inc");
require_once("inc/lang.inc");

$ctr = new controller();

if (!isset($_SESSION['userid']) and (empty($_POST)))
{
    $ctr->view("login");
}
if (!isset($_SESSION['userid']) and ($_POST['action']=='auth'))
    if ($ctr->authenticate($_POST['userid'],$_POST['password']))
        $_GET['action']='start';
    else
        $ctr->view("login");

if (isset($_SESSION['userid']))
{
    switch ($_GET['action'])
    {
        case 'start':
        {
            $ctr->view("start");
            break;
        }
        case 'logout':
        {
            $ctr->logout();
            break;        
        }

        default:
        {
            $ctr->view("start");
            break;
        }
    }

}
?>
