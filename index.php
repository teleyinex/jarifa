<?php 
require_once("inc/html.inc");
//require_once("inc/auth.inc");
require_once("views/class_view.inc");
require_once("controller/controller.inc");
require_once("inc/lang.inc");

$ctr = new controller();
// If $_POST is empty, OGM should show the login screen
if (empty($_POST) and (!isset($_SESSION['userid'])))
{
    $view= new login($language);
    $view->form();
}
else
{
    switch($_POST['action'])
    {
    case 'authenticate':
    {
        if ($ctr->authenticate($_POST['userid'],$_POST['password']))
        {
             printf($_SESSION['userid']);
             print_r($_SESSION);
        }
        else
        {
            printf(gettext("ERROR, the username or password is incorrect.")); 
        }
    }
    default:
    {
        // If the user has log in, it can perform different accions based on this role
        if (isset($_SESSION['user']))
            switch ($_SESSION['role'])
            {
                case 'supplier':
                {
                    printf("SUPPLIER");
                }
                case 'allocator':
                {
                    printf("ALLOCATOR");
                }
            }
    }
    }
}

?>
