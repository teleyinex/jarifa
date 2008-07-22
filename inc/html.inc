<?php 
require_once("model/data.inc");

class html
{
    function __construct($language='es_ES.utf8')
    {
        putenv("LANG=$language");
        setlocale(LC_ALL, $language);
        $domain = 'messages';
        bindtextdomain($domain, "locale"); 
        textdomain($domain);
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
            printf('<li><a href="index.php?action=machine">'.gettext("Machines").'</a>
                <li><a href="index.php?action=pool">'.gettext("Pools").'</a>
                <li><a href="index.php?action=stat">'.gettext("Statistics").'</a>');
            break;
        }
        case 'root':
        {
            printf('<li><a href="index.php?action=project">'.gettext("Projects").'</a>
                <li><a href="index.php?action=machine">'.gettext("Machines").'</a>
                <li><a href="index.php?action=pool">'.gettext("Pools").'</a>
                <li><a href="index.php?action=stat">'.gettext("Statistics").'</a>
                ');
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
        printf('<table border=1 align=center>');
        printf('<tr><td>Name</td><td>Share</td><td align=center colspan="2" rowspan="1">Actions</td></tr>');
        $row = count($obj);
        for ($i=0 ; $i<$row;$i++)
        {
            printf ("<tr>");
            printf ('<td align=center><a href=index.php?action=ed_project&id='.$obj[$i]->id.'>'.$obj[$i]->id.'</a></td><td align=center>'.$obj[$i]->share.'<td>Edit</td><td>Delete</td>');
            printf ("\n</tr>");
        }
        printf("</table>");
    }

    function table_project_edit($obj)
    {
        printf('<form method="post" action="index.php?action=up_project&id='.$obj->id.'" name="project">');
        printf('<table border=1 align=center>');
        printf('<tr><td>Name</td><td>Share</td><td>Actions</td></tr>');
        printf ('<tr>');
        printf ('<td align=center><a href=index.php?action=ed_project&id='.$obj->id.'>'.$obj->id.'</a></td><td><input
            size="2" value="'.$obj->share.'"><td align=center><input name="delete" type="checkbox"></td>');
        printf ("\n</tr>");
        printf('<tr align="center"> <td colspan="3" rowspan="1"><input name="submit" type="submit"></td> </tr>');
        printf("</table>");

    }


    function footer()
    {
        printf('
            </div>
            </body>
            </html>
        ');
    }

}
?>
