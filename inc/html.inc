<?php 
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
