<?php 
require_once("inc/html.inc");
//require_once("inc/auth.inc");
require_once("views/class_view.inc");
require_once("controller/controller.inc");
require_once("inc/lang.inc");

$ctr = new controller();

if (!isset($_SESSION['userid']))
{
    $ctr->view("login");
}
else
{
    switch ($_GET['action'])
    {
        case 'start':
            $ctr->view('start');
            break;

        case 'logout':
            $ctr->logout();
            break;

    
    }
}

?>
