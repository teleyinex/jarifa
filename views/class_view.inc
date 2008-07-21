<?php 
require_once("inc/html.inc");

class login extends html
{
    public $userid;
    public $password;

    function form()
    {
        $this->header();
        printf('
        <form method="post" action="index.php" name="login">
        <input type="hidden" name="action" value="authenticate">
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
?>
