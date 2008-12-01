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
require_once("views/volunteer.inc");
require_once("controller/controller.inc");
require_once("inc/lang.inc");

$ctr = new controller("en_US.utf8");
// This for non authenticated users. Only anonymous users can create Volunteer accounts.
if (!isset($_SESSION['userid']) and (empty($_POST)))
{
    $ctr->view("volunteer");
}

switch ($_GET['action'])
{
    // Insert the new volunteer
    case 'in_volunteer':
    {
      if ($ctr->volunteer->insert($_POST))
                    // If everything went OK, thank the user
                    $ctr->view("volunteer",$_SESSION['role']="volunteer","thanks");
                else
                    // Else show an error page
                    $ctr->view("error",$_SESSION['role']="volunteer",null,gettext("Empty fields on the insert form or password mismatch."));
                break;

    }
}
?>
