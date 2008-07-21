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

    function header()
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
            ');
    }


    function footer()
    {
        printf('
            </body>
            </html>
        ');
    }

}
?>
