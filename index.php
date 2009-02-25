<?php 
/* 
Copyright 2008 Daniel Lombraña González, David P. Anderson, Francisco Fernández de Vega

This file is part of Jarifa.

Jarifa is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Jarifa is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with Jarifa.  If not, see <http://www.gnu.org/licenses/>.
*/

require_once("inc/html.inc");
require_once("views/default.inc");
require_once("controller/controller.inc");

$ctr = new controller("es_ES.utf8");
// If the user has not been authenticated and there is not any action, the login screen must be shown
if (!isset($_SESSION['userid']) and (empty($_POST) and empty($_GET)))
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

        case 'stat':
        {
            $ctr->view("stat",$_SESSION['role'],'start');
            break;
        }

        case 'st_credit':
        {
            $ctr->view("stat",$_SESSION['role'],'credit');
            break;
        }

        case 'st_gflops':
        {
            $ctr->view("stat",$_SESSION['role'],'gflops');
            break;
        }

        
        // Users action
        case 'user':
        {
            $ctr->view("user",$_SESSION['role']);
            break;
        }

        case 'ed_user':
        {
            $ctr->view("user",$_SESSION['role'],'edit');
            break;
        }

        case 'vin_user':
        {
            $ctr->view("user",$_SESSION['role'],'insert');
            break;
        }
        case 'up_user':
        {
            if ($_POST['delete'])
                {
                $err=$ctr->user->delete($_GET['id']);
                }
            else
                {
                $err=$ctr->user->update($_GET['id'],$_POST,&$err_msg);
                }
            if ($err != null) $ctr->view("user",$_SESSION['role']);
            else $ctr->view("error",$_SESSION['role'],null,$err_msg);
                
            break;
        }

        case 'in_user':
        {
                if ($ctr->user->insert($_POST,&$err_msg))
                    $ctr->view("user",$_SESSION['role']);
                else
                    $ctr->view("error",$_SESSION['role'],null,$err_msg);
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

        case 'vin_project':
        {
            $ctr->view("project",$_SESSION['role'],'insert');
            break;
        }
        case 'up_project':
        {
            if ($_POST['delete'])
                {
                $err=$ctr->project->delete($_GET['id']);
                }
            else
                {
                $err=$ctr->project->update($_GET['id'],$_POST);
                }
            if ($err != null) $ctr->view("project",$_SESSION['role']);
            else $ctr->view("error",$_SESSION['role'],null,gettext("The project can not be updated or deleted"));
                
            break;
        }

        case 'in_project':
        {
                $weak_auth = $ctr->create_boinc_user($_POST['url']);
                if (!empty($weak_auth))
                {
                    $_POST['authenticator'] = $weak_auth;
                    if ($ctr->project->insert($_POST))
                        $ctr->view("project",$_SESSION['role']);
                    else
                        $ctr->view("error",$_SESSION['role'],null,gettext("Empty fields on the insert form."));
                }
                else
                        $ctr->view("error",$_SESSION['role'],null,gettext("The BOINC project is not available."));
                break;
        }

        // Hosts action
        case 'host':
        {
            $ctr->view("host",$_SESSION['role']);
            break;
        }

        case 'ed_host':
        {
            $ctr->view("host",$_SESSION['role'],'edit');
            break;
        }

        case 'up_host':
        {
            if ($_POST['delete'])
                {
                $err=$ctr->host->delete($_GET['id']);
                }
            else
                {
                $err=$ctr->host->update($_GET['id'],$_POST);
                }
            if ($err != null) $ctr->view("host",$_SESSION['role']);
            else $ctr->view("error",$_SESSION['role'],null,gettext("The host can not be updated or deleted"));
            break;
        }

        
        // Pools action
        case 'pool':
        {
            $ctr->view("pool",$_SESSION['role']);
            break;
        }

        case 'ed_pool':
        {
            $ctr->view("pool",$_SESSION['role'],'edit');
            break;
        }

        case 'up_pool':
        {
            if ($_POST['delete'])
                {
                $err=$ctr->pool->delete($_GET['id']);
                }
            else
                {
                $err=$ctr->pool->update($_GET['id'],$_POST);
                }
            if ($err != null) $ctr->view("pool",$_SESSION['role']);
            else $ctr->view("error",$_SESSION['role'],null,gettext("The pool can not be updated or deleted"));
            break;
        }

        case 'vin_pool':
        {
            $ctr->view("pool",$_SESSION['role'],'insert');
            break;
        }

        case 'in_pool':
        {
                if ($ctr->pool->insert($_POST))
                    $ctr->view("pool",$_SESSION['role']);
                else
                    $ctr->view("error",$_SESSION['role'],null,gettext("Empty fields on the insert form."));
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
else
{
    switch($_GET['action'])
    {
        case 'ranking':
        {
            $ctr->view("ranking",null);
            break;
        }

        case 'map':
        {
            $ctr->view("map",null);
            break;
        }

        case 'volunteer':
        {
            $ctr->view("volunteer",null);
            break;
        }

        case 'in_volunteer':
        {
            $ctr->view("volunteer",null,"in_volunteer");
            break;
        }

    }
}
?>
