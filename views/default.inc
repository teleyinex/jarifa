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
require_once("ranking.inc");
require_once("inc/db_conn.inc");

class login_page extends html
{
    public $userid;
    public $password;

    function __construct($language="en_US.utf8")
    {
        putenv("LANG=$language");
        setlocale(LC_ALL, $language);
        $domain = 'messages';
        bindtextdomain($domain, "lang");
        textdomain($domain);
        $this->load_config();
    }


    function page($nvolunteers="0",$ncomputers="0",$nsuppliers="0")
    {
        $this->header();
        printf('
        <form method="post" action="index.php" name="login">
        <input type="hidden" name="action" value="auth">
        <table  style="text-align: left; margin-left: auto; margin-right: auto; height: 66px; width: 308px;"
         border="0" cellpadding="2" cellspacing="2">
         <tbody>
           <tr>
             <td> %s &nbsp;</td>
             <td><input name="userid"></td>
           </tr>
           <tr>
             <td>%s</td>
             <td><input name="password" type="password"></td>
           </tr>
           <tr>
             <td colspan="2" rowspan="1"></td>
           </tr>
           <tr>
           <td style="width: 117px; text-align: center; vertical-align: middle;"
 colspan="2" rowspan="1">
           <input type="submit"/>
           </td>
           </tr>
         </tbody>
        </table>
        &nbsp;</form>
        ',gettext("User"),gettext("Password"));
        $major = intval($this->conf->account_manager->min_core_client_version/100);
        $minor = $this->conf->account_manager->min_core_client_version % 100;
        printf("<div id=requirements align=center>".gettext("Mininum BOINC core client version: "));
        printf ("$major.$minor</div>");
        printf("<div id=volunteer align=center>".gettext("If you want to collaborate with us").", <a href=index.php?action=volunteer>".gettext("Join us")."</a>");
        if ($nvolunteers != 0) 
        {
            printf("<br>");
            printf(gettext("Powered by")." ".$nvolunteers." ".gettext("Volunteers"));
            printf(" ".gettext("and")." $nsuppliers ".gettext("Suppliers"));
            printf("<br>".gettext("Computers:")." $ncomputers");
        }
        else
        {
            printf("<br>");
            printf(gettext("Powered by")." ".$nsuppliers." ".gettext("Suppliers"));
            printf("<br>".gettext("Computers:")." $ncomputers");
               
        }
        printf("<br><a href=index.php?action=ranking>".gettext("Best volunteers")."</a>");
        printf("<br><a href=index.php?action=map>".gettext("Map")."</a>");
        printf("</div>");
        $this->footer();
    }

}

class error_page extends html
{
    public $role;
    public $msg;

    function __construct($role='default',$msg=null)
    {
        $this->role = $role;
        $this->msg = $msg;
        $this->load_config();
    }

    function page()
    {
        $this->header(gettext("ERROR"));
        $this->menu($this->role,gettext("An error has occurred"));
        printf("<div id=error>".$this->msg."</div>");
        $this->footer();
    }
}

class start_page extends html
{
    public $role;

    function __construct($role='default')
    {
        $this->role = $role;
        $this->load_config();
        $this->db = new DbConn();
        $this->db->init_conn($this->db_user,$this->db_password,$this->db_host,$this->db_name);

    }

    function load_config()
      {
          $conf = simplexml_load_file("conf/jarifa.conf");
          $this->db_user = $conf->db->user;
          $this->db_password = $conf->db->password;
          $this->db_host = $conf->db->host;
          $this->db_name = $conf->db->name;
          $this->language = $conf->account_manager->language;
          $this->logo = $conf->account_manager->logo;
      }


    function welcome_table()
    {
        // A ranking object is created for obtaining the user or supplier stats
        $ranking = new HTMLranking($this->language,"",$this->db);
        print('<div align="center">');
        $ranking->table_ranking_volunteers($_SESSION['userid']);
        print('</div>');
    }



    function page()
    {
        switch ($this->role)
        {
            case 'root':
            {
                $this->header(gettext("Welcome"). ' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Choose an action from the left panel"));
                $this->empty_table();
                $this->footer();
                break;
            }
            case 'allocator':
            {
                $this->header(gettext("Welcome"). ' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Choose an action from the left panel"));
                $this->empty_table();
                $this->footer();
                break;
            }
            case 'supplier':
            {
                $this->header(gettext("Welcome").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Choose an action from the left panel"));
                $this->empty_table();
                $this->footer();
                break;
            }
            case 'volunteer':
            {
                $this->header(gettext("Welcome").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Choose an action from the left panel"));
                $this->welcome_table();
                $this->footer();
                break;
            }


        }
    }
}
?>
