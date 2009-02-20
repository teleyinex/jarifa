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

class HTMLuser extends html
{
    public $role;
    public $db;
    public $table='user';
    public $class='user';
    public $language;

    function __construct($language,$role=null,$db)
    {
        $this->language=$language;
        putenv("LANG=$this->language");
        setlocale(LC_ALL, $this->language);
        $domain = 'messages';
        bindtextdomain($domain, "lang");
        textdomain($domain);
        $this->role = $role;
        $this->db = $db;
        $this->load_config();
    }

    function page($view=null,$values=null)
    {
        switch ($view)
        {
            case 'edit':
            {
                $this->header(gettext("Users"));
                $this->menu($this->role,gettext("Editing user"));
                $obj = $this->get($values['id']);
                $this->html_edit($obj);
                break;
            }

            case 'insert':
            {
                $this->header(gettext("Users"));
                $this->menu($this->role,gettext("Insert a new user"));
                $this->html_insert(); 
                break;
            }
            default:
            {
                $this->header(gettext("Welcome").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Available users"));
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
                 <td align="center">'.gettext("Repeat Password").'</td>
                 <td align="center"><input name="password2" type="password"></td>
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

    function html_insert()
    {
        printf('<form method="post" action="index.php?action=in_user" name="user">');
        printf('<table border="1" align="center" cellpading="4" class="form">');
        printf('<tr>
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
                 <td align="center">'.gettext("Role").'</td>
                 <td align="center"><select name="role">');
                 printf('<option value="root">'.gettext("root").'</option>');
                 printf('<option value="supplier">'.gettext("supplier").'</option>');
                 printf('<option value="allocator">'.gettext("allocator").'</option>');
                 printf('
                 </select></td>
                </tr>
                <tr> 
                <td align="center">'.gettext("Supplier").'
                <td align="center"><input name="supplier"></td>
                </tr>
                <tr>
                <td align="center">'.gettext("E-mail").'
                <td align="center"><input name="email"></td>
                </tr>
                <tr>
                <td align="center">'.gettext("City").'
                <td align="center"><input name="city"></td>
                </tr>
                <tr>
                <td align="center">'.gettext("State").'
                <td align="center"><input name="state"></td>
                </tr>
                <tr>
                <td align="center">'.gettext("Country").'
                <td align="center"><input name="country"></td>
                </tr>
                <tr>
                <td align="center">'.gettext("Postal code").'
                <td align="center"><input name="postalcode"></td>
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
          //if (empty($values['password']))
          $query = 'name="'.$values['name'].'",
                    role="'.$values['role'].'",
                    password="'.md5($values['password']).'",
                    supplier="'.$values['supplier'].'",
                    supp_auth="'.md5($values['password'].$values['name']).'"';
          //else
          //{
          //$query = 'name="'.$values['name'].'",
          //          password="'.md5($values['password']).'",
          //          role="'.$values['role'].'",
          //          supplier="'.$values['supplier'].'",
          //          supp_auth="'.md5($values['password'].$values['name']).'"';

          //}
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

      function insert($values,&$err_msg)
      {
          $insert=True;
          $err_msg=null;
          $create_default_pool=False;
          if (empty($values['name'])) 
          {
              $insert=False;
              $err_msg=gettext("User name empty");
          }
          // Temporary fix for a bug on BOINC client when generating password hash. This will be removed in a future,
          // allowing uppercase and lowercase letters on user name.
          if (strtolower($values['name'])!=$values['name'])
          {
              $insert=False;
              if ($err_msg != null) $err_msg = $err_msg." ".gettext("and")." ".gettext("User name must be lowercase");
              else $err_msg=gettext("User name must be lowercase");
          }

          if (empty($values['password'])) 
          {
              $insert=False;
              if ($err_msg != null) $err_msg = $err_msg."<br>".gettext("Password empty");
              else $err_msg=gettext("Password empty");

          }
          if (empty($values['password2']))
          {
              $insert=False;
              if ($err_msg != null) $err_msg = $err_msg."<br>".gettext("Password empty");
              else $err_msg=gettext("Password empty");

          }
          if (empty($values['role']))
          {
              $insert=False;
              if ($err_msg != null) $err_msg = $err_msg."<br>".gettext("Role empty");
              else $err_msg=gettext("Role empty");

          }
          if (empty($values['supplier']))
          {
              $insert=False;
              if ($err_msg != null) $err_msg = $err_msg."<br>".gettext("Supplier empty");
              else $err_msg=gettext("Supplier empty");

          }
          if (empty($values['email'])) 
          {
              $insert=False;
              if ($err_msg != null) $err_msg = $err_msg."<br>".gettext("E-mail empty");
              else $err_msg=gettext("E-mail empty");

          }
          if ($values['password']!=$values['password2']) 
          {
              $insert=False;
              if ($err_msg != null) $err_msg = $err_msg."<br>".gettext("Password missmatch");
              else $err_msg=gettext("Password missmatch");

          }
          // Exists the user name on the DB?
          $check_username = $this->db->lookup_fields($this->table,$this->class,'*','name="'.$values['name'].'"');
          if (!(empty($check_username)))
          {
                $insert=False;
                if ($err_msg != null) $err_msg = $err_msg."<br>".gettext("User name exists on the site");
                else $err_msg=gettext("User name exists on the site");
          }

          // Exists the e-mail on the DB?
          $check_email = $this->db->lookup_fields($this->table,$this->class,'*','email="'.$values['email'].'"');
          if (!(empty($check_email)))
          {
                $insert=False;
                if ($err_msg != null) $err_msg = $err_msg."<br>".gettext("E-mail exists on the site");
                else $err_msg=gettext("E-mail exists on the site");
          }


          if ($insert)
          {
            $pool = $this->db->lookup_fields('pool','pool','*','name="default" and supplier="'.$values['supplier'].'"');
            if (empty($pool)) $create_default_pool = True;
            $query = '(name,password,role,supplier,supp_auth,email,city,state,country,postalcode,date) values ("'
              .$values['name'].'",
              "'.md5($values['password']).'","'.$values['role'].'","'.$values['supplier'].'","'.md5($values['password'].$values['name']).'","'.$values['email'].'","'.$values['city'].'","'.$values['state'].'","'.$values['country'].'","'.$values['postalcode'].'",CURDATE())';
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
