<?php 
require_once("inc/html.inc");
require_once("views/class_view.inc");
require_once("controller/controller.inc");
require_once("inc/lang.inc");

$ctr = new controller();

switch ($_POST['action'])
{
    case 'auth':
    {
        $ctr->authenticate($_POST['userid'],$_POST['password']);
        header("Location: index.php?action=start");
    }
}

?>
