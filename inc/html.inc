<?php 
require_once("model/data.inc");

class html
{
    public $logo;

    function __construct($language='es_ES.utf8')
    {
        putenv("LANG=$language");
        setlocale(LC_ALL, $language);
        $domain = 'messages';
        bindtextdomain($domain, "locale"); 
        textdomain($domain);
    }


    function load_config()
    {
        $conf = simplexml_load_file("conf/ogm.conf");
        $this->logo = $conf->account_manager->logo;
    }


    function header($msg=null)
    {
        $title = gettext("OGM Server");
        printf('
            <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
            <html>
            <head>
              <meta content="text/html; charset=utf-8"  http-equiv="content-type">
             <title>'.$title.'</title>
             <style type="text/css">
             @import url("views/style.css");
             </style>
            </head> 
            <body>
            <div id="container">
            <div id="logo" align="center">
            <a href=index.php><img src='.$this->logo.' border="0"></a>
            </div>
            <div id="header">
                <h1>'.$msg.'</h1> 
            </div>
            ');
    }

    function menu($role=null,$h2=null)
    {
        printf('
                <div id="menu">
                <ul id="menu_list">
                <li><a href="index.php">'.gettext("Start").'</a>
                ');
        switch ($role)
        {
        case 'allocator':
        {
            printf('<li><a href="index.php?action=project">'.gettext("Projects").'</a>');
            break;
        }
        case 'supplier':
        {
            printf('<li><a href="index.php?action=host">'.gettext("Hosts").'</a>
                <li><a href="index.php?action=pool">'.gettext("Pools").'</a>');
                //<li><a href="index.php?action=stat">'.gettext("Statistics").'</a>');
            break;
        }
        case 'root':
        {
            printf('<li><a href="index.php?action=project">'.gettext("Projects").'</a>
                <li><a href="index.php?action=host">'.gettext("Hosts").'</a>
                <li><a href="index.php?action=pool">'.gettext("Pools").'</a>
                <li><a href="index.php?action=user">'.gettext("Users").'</a>
                ');
                //<li><a href="index.php?action=stat">'.gettext("Statistics").'</a>
            break;
        }

        }
        printf ('
            <li><a href="index.php?action=logout">'.gettext("Log Out").'</a>
            </ul></div>
            <div id="content">
            <h2>'.$h2.'</h2> 
            ');

    }


    function table_project($obj)
    {
        // acction=up -> Update Project -> up
        printf('<table border=1 align=center cellpadding="4">');
        printf('<tr><td align="center">Name</td><td align="center">Share</td></tr>');
        $row = count($obj);
        for ($i=0 ; $i<$row;$i++)
        {
            printf ("<tr>");
            printf ('<td align=center><a href=index.php?action=ed_project&id='.$obj[$i]->id.'>'.$obj[$i]->name.'</a></td><td align=center>'.$obj[$i]->share.'');
            printf ("\n</tr>");
        }
        printf("</table>");
        printf('<div style="text-align: center;"><a href=index.php?action=vin_project>'.gettext("New Project").'</a></div>');
    }

    function table_project_edit($obj)
    {
        printf('<form method="post" action="index.php?action=up_project&id='.$obj->id.'" name="project">');
        printf('<table border=1 align=center cellpading="4">');
        printf('<tr><td align="center">Name</td><td align="center">Share</td><td align="center">Delete</td></tr>');
        printf ('<tr>');
        printf ('<td align=center><a href=index.php?action=ed_project&id='.$obj->id.'>'.$obj->name.'</a></td><td
            align="center"><input name="share" size="2" value="'.$obj->share.'"><td align=center><input name="delete" type="checkbox"></td>');
        printf ("\n</tr>");
        printf('<tr align="center"> <td colspan="3" rowspan="1"><input name="submit" type="submit"></td> </tr>');
        printf("</table>");
        printf("</form>");

    }


    function empty_table()
    {
        printf('
            <table style="text-align: left; width: 100px; height: 391px;"
             border="0" cellpadding="2" cellspacing="2">
                <tbody>
                    <tr>
                    <td></td>
                    </tr>
                </tbody>
            </table>
            '); 
    }


    function footer()
    {
        printf('
            </div>
            <div id="credits" align="center">
            <a href="http://jigsaw.w3.org/css-validator/"><img style="border:0;width:88px;height:31px" src="img/vcss"/></a>
            </div>
            </body>
            </html>
        ');
    }

}
?>
