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
require_once("inc/recaptchalib.php");

class HTMLvolunteer extends html
{
    public $role;
    public $db;
    public $table='user';
    public $class='user';
    public $language;
    public $recap_pubkey = "";
    public $recap_prikey = "";

    function __construct($language,$role=null,$db)
    {
        $this->language=$language;
        putenv("LANG=$this->language");
        setlocale(LC_ALL, $this->language);
        $domain = 'text';
        bindtextdomain($domain, "lang");
        textdomain($domain);
        $this->role = "volunteer";
        $this->db = $db;
        $this->load_config();
    }

    function page($view=null,$values=null)
    {
        switch ($view)
        {
            case 'thanks':
            {
                $this->header(gettext("You have joined Jarifa"));
                $this->html_thanks();
                break;
            }

            default:
            {
                $this->header(gettext("Welcome volunteer"));
                $this->html_insert();
                break;
            }

        }

    }

    function table($obj)
    {
        printf('<table border=1 align=center cellpadding="4" class="form">');
        printf('<tr class="Hd"><td align="center">'.gettext("User ID").'</td><td align="center">'.gettext("Role").'</td><td
            align="center">'.gettext("Supplier").'</td></tr>');
        $row = count($obj);
        for ($i=0 ; $i<$row;$i++)
        {
            printf ("<tr>");
            printf ('<td align=center><a href=index.php?action=ed_user&id='.$obj[$i]->id.'>'.$obj[$i]->name.'</a></td>
                <td align="center">'.$obj[$i]->role.'</td>
                <td align="center">'.$obj[$i]->supplier.'</td>');
            printf ("\n</tr>");
        }
        printf("</table>");
        printf('<div style="text-align: center;"><a href=index.php?action=vin_user>'.gettext("New User").'</a></div>');
        $this->footer();
    }

    function html_edit($obj)
    {
        printf('<form method="post" action="index.php?action=up_user&id='.$obj->id.'" name="user">');
        printf('<table border=1 align=center cellpading="4" class="form">');
        printf('<tr>
                 <td align="center">'.gettext("User ID").'</td>
                 <td align=center><input name="name" value="'.$obj->name.'"></td>
                </tr>
                <tr>
                 <td align="center">'.gettext("Password").'</td>
                 <td align="center"><input name="password" type="password"></td>
                </tr>
                <tr>
                 <td align="center">'.gettext("Role").'</td>
                 <td align="center"><select name="role">');
                 if ($obj->role == 'root') printf('<option value="root" selected="selected">'.gettext("root").'</option>');
                 else printf('<option value="root">'.gettext("root").'</option>');
                 if ($obj->role == 'supplier') printf('<option value="supplier" selected="selected">'.gettext("supplier").'</option>');
                 else printf('<option value="supplier">'.gettext("supplier").'</option>');
                 if ($obj->role == 'allocator') printf('<option value="allocator" selected="selected">'.gettext("allocator").'</option>');
                 else printf('<option value="allocator">'.gettext("allocator").'</option>');
                 printf('
                 </select></td>
                </tr>
                <tr> 
                <td align="center">'.gettext("Supplier").'
                <td align="center"><input name="supplier" value="'.$obj->supplier.'"></td>
                </tr>
                <tr>
                    <td align="center">'.gettext("Delete").'</td>
            <td align="center"><input name="delete" type="checkbox"></td>');
        printf ("</tr>");
        printf('<tr align="center"> <td colspan="3" rowspan="1"><input name="submit" type="submit"></td> </tr>');
        printf("</table>");
        printf("</form>");
        $this->footer();
    }

    function html_thanks()
    {
        printf("<div id=thanks align=center>".gettext("<h2>Thanks for joining Jarifa, now you should attach your BOINC client
            to Jarifa.</h2>"));
        printf(gettext("<h2>Checkout the following link for a <a href=http://jarifa.unex.es/wiki/wiki/QuickstartClient>quick start</a>
            guide.")."</div></h2>");
        $this->footer();
    }

    function html_insert()
    {
        printf('<div id="content">');
        printf('<form method="post" action="volunteer.php?action=in_volunteer" name="user">');
        printf('<table border="1" align="center" cellpading="4" class="form">');
        printf('
                <tr>
                <td colspan=2>
                Fill in the following form, to become a Jarifa user
                </td>
                </tr>
                <tr>
                 <td align="center">'.gettext("User ID").'</td>
                 <td align="center"><input name="name"></td>
                </tr>
                <tr>
                 <td align="center">'.gettext("Password").'</td>
                 <td align="center"><input name="password" type="password"></td>
                </tr>
                <tr>
                 <td align="center">'.gettext("Repeat Password").'</td>
                 <td align="center"><input name="password2" type="password"></td>
                </tr>
                <tr>
                 <td align="center" colspan=2>'.recaptcha_get_html($this->recap_pubkey).'
                </tr>
                 ');
        printf('<tr align="center"> <td colspan="3" rowspan="1"><input name="submit" type="submit"></td> </tr>');
        printf('</table>');
        printf('</form>');
        $this->footer();
    }

    function lists()
      {
          $obj = $this->db->enum_fields($this->table,$this->class,'*','1', 'order by id');
          return($obj);
      }

      function get($id)
      {
          return ($this->db->lookup_fields($this->table,$this->class,'*','id="'.$id.'"'));
      }

      function update($id,$values)
      {
          if (empty($values['password']))
          {
          $query = 'name="'.$values['name'].'",
                    role="'.$values['role'].'",
                    supplier="'.$values['supplier'].'"';
          }
          else
          {
          $query = 'name="'.$values['name'].'",
                    password="'.md5($values['password']).'",
                    role="'.$values['role'].'",
                    supplier="'.$values['supplier'].'"';

          }
        $obj = $this->db->lookup_id($id,$this->table,$this->class);
        return($this->db->update($obj,$this->table,$query));
      }

      function delete($id)
      {
        $user = $this->db->lookup_fields('user','user','*','id='.$id);
        $suppliers = $this->db->enum_fields('user','user','supplier','supplier="'.$user->supplier.'"','order by supplier');
        if (count($suppliers)>1)
        {
            $obj = $this->db->lookup_id($id,$this->table,$this->class);
            return($this->db->delete($obj,$this->table));
        }
        else
        {
            $obj = $this->db->lookup_id($id,$this->table,$this->class);
            $this->db->delete($obj,$this->table);
            $default_pool = $this->db->lookup_fields('pool','pool','*','name="default" and supplier="'.$user->supplier.'"');
            return($this->db->delete($default_pool,'pool'));
        }
      }

      function insert($values)
      {
          $insert=True;
          $create_default_pool=False;
          if (empty($values['name'])) $insert=False;
          if (empty($values['password'])) $insert=False;
          if (empty($values['password2'])) $insert=False;
          // If passwords are different, give an error
          if ($values['password']!=$values['password2']) $insert=False;
          $values['role'] = "volunteer";
          $values['supplier'] = "Volunteer";
          // reCaptcha checking
          $resp = recaptcha_check_answer ($this->recap_prikey,$_SERVER["REMOTE_ADDR"], $_POST["recaptcha_challenge_field"],$_POST["recaptcha_response_field"]);
          if (!$resp->is_valid) {
              $insert = False;
              die ("The reCAPTCHA wasn't entered correctly. Go back and try it again." . "(reCAPTCHA said: " . $resp->error . ")");
          }

          if ($insert)
          {
            $pool = $this->db->lookup_fields('pool','pool','*','name="default" and supplier="'.$values['supplier'].'"');
            if (empty($pool)) $create_default_pool = True;
            $query = '(name,password,role,supplier,supp_auth) values ("'
              .$values['name'].'",
              "'.md5($values['password']).'","'.$values['role'].'","'.$values['supplier'].'","'.md5($values['password'].$values['name']).'")';
            $res = $this->db->insert($this->table, $query);
            if ($res and $create_default_pool)
            {
                {
                    $query = '(name,supplier) values ("default","'.$values['supplier'].'")';
                    return($this->db->insert('pool',$query));
                }
            }
            return($res);
          }
          else
              return(False);
      }
}
?>
