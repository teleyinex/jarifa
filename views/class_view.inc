<?php 
require_once("inc/html.inc");

class login_page extends html
{
    public $userid;
    public $password;

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

class start_page extends html
{
    public $role;

    function __construct($role='default')
    {
        $this->role = $role;
    }

    function page()
    {
        switch ($this->role)
        {
            case 'root':
            {
                $this->header(gettext("Wellcome"). ' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Choose an action from the left panel"));
                break;
            }
            case 'allocator':
            {
                $this->header(gettext("Wellcome"). ' '.$_SESSION['userid']);
 
                $this->menu($this->role,gettext("Choose an action from the left panel"));
                break;
            }
            case 'supplier':
            {
                $this->header(gettext("Wellcome").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Choose an action from the left panel"));
                break;
            }

        }
    }
}

class project_page extends html
{
    public $action;
    public $role;
    function __construct($role=null,$action=null)
    {
        $this->action=$action;
        $this->role=$role;
    }

    function page()
    {
        switch ($this->action)
        {
            default:
            {
                $this->header(gettext("Wellcome").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Available BOINC projects"));
                break;
            }
        
        }
    
    }


}
?>