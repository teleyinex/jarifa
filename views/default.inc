<?php 
require_once("inc/html.inc");

class login_page extends html
{
    public $userid;
    public $password;

    function __construct($language="es_ES.utf8")
    {
        putenv("LANG=$language");
        setlocale(LC_ALL, $language);
        $domain = 'text';
        bindtextdomain($domain, "lang");
        textdomain($domain);
        $this->load_config();
    }


    function page()
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
    }

    function page()
    {
        switch ($this->role)
        {
            case 'root':
            {
                $this->header(gettext("Wellcome"). ' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Choose an action from the left panel"));
                $this->empty_table();
                $this->footer();
                break;
            }
            case 'allocator':
            {
                $this->header(gettext("Wellcome"). ' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Choose an action from the left panel"));
                $this->empty_table();
                $this->footer();
                break;
            }
            case 'supplier':
            {
                $this->header(gettext("Wellcome").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Choose an action from the left panel"));
                $this->empty_table();
                $this->footer;
                break;
            }

        }
    }
}
?>
