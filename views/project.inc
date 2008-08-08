<?php 
/* 
Copyright 2008 Daniel Lombraña González, David Anderson, Franisco Fernández de Vega

This file is part of OGM.

OGM is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OGM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with OGM.  If not, see <http://www.gnu.org/licenses/>.
*/

require_once("inc/html.inc");

class HTMLproject extends html
{
    public $role;
    public $db;
    public $table='project';
    public $class='project';
    public $language;

    function __construct($language,$role=null,$db)
    {
        $this->language=$language;
        putenv("LANG=$this->language");
        setlocale(LC_ALL, $this->language);
        $domain = 'text';
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
                $this->header(gettext("Projects"));
                $this->menu($this->role,gettext("Editing project"));
                $obj = $this->get($values['id']);
                $this->html_edit($obj);
                break;
            }

            case 'insert':
            {
                $this->header(gettext("Projects"));
                $this->menu($this->role,gettext("Insert a new project"));
                $this->html_insert(); 
                break;
            }
            default:
            {
                $this->header(gettext("Wellcome").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Available BOINC projects"));
                break;
            }

        }

    }

    function table($obj)
    {
        printf('<table border=1 align=center cellpadding="4" class="form"><tbody>');
        printf('<tr class="Hd"><td align="center">'.gettext("Name").'</td>
            <td align="center">'.gettext("Share").'</td>
            <td align="center">'.gettext("Detach").'</td>
            <td align="center">'.gettext("Update").'</td>
            </tr>');
        $row = count($obj);
        for ($i=0 ; $i<$row;$i++)
        {
            printf ("<tr>");
            printf ('<td align=center><a href=index.php?action=ed_project&id='.$obj[$i]->id.'>'.$obj[$i]->name.'</a></td>
                     <td align=center>'.$obj[$i]->share.'</td>');
            if ($obj[$i]->detach) printf('<td align="center">'.gettext("Yes").'</td>');
            else printf('<td align="center">'.gettext("No").'</td>');
            if ($obj[$i]->update) printf('<td align="center">'.gettext("Yes").'</td>');
            else printf('<td align="center">'.gettext("No").'</td>');
            printf ("\n</tr>");
        }
        printf("</tbody></table>");
        printf('<div style="text-align: center;"><a href=index.php?action=vin_project>'.gettext("New Project").'</a></div>');
        $this->footer();
    }

    function html_edit($obj)
    {
        printf('<form method="post" action="index.php?action=up_project&id='.$obj->id.'" name="project">');
        printf('<table border=1 align=center cellpading="4" class="form">');
        printf('<tr class="Hd"><td align="center">'.gettext("Name").'</td><td align="center">'.gettext("Share").'</td>
                    <td align="center">'.gettext("Detach").'</td><td align="center">'.gettext("Update").'</td>
                    <td align="center">'.gettext("Delete").'</td>
                    
                    </tr>');
        printf ('<tr>');
        printf ('<td align=center><a href=index.php?action=ed_project&id='.$obj->id.'>'.$obj->name.'</a></td><td
            align="center"><input name="share" size="2" value="'.$obj->share.'">');
        if ($obj->detach) printf ('<td align="center"><input name="detach" type="checkbox" CHECKED></td>');
        else printf ('<td align="center"><input name="detach" type="checkbox"></td>');
        if ($obj->update) printf ('<td align="center"><input name="update" type="checkbox" CHECKED></td>');
        else printf ('<td align="center"><input name="update" type="checkbox"></td>');
        printf('<td align=center><input name="delete" type="checkbox"></td>');
        printf ("\n</tr>");
        printf('<tr align="center"> <td colspan="5" rowspan="1"><input name="submit" type="submit"></td> </tr>');
        printf("</table>");
        printf("</form>");
        $this->footer();
    }

    function html_insert()
    {
    printf('
    <form method="post" action="index.php?action=in_project">
    <table align="center" style="text-align: left; width: 100px;" border="1"  cellpadding="2" cellspacing="2" class="form">
        <tbody>
          <tr>
            <td>'.gettext("Name").'</td>
            <td><input size="30" maxlength="255" name="name"></td>
          </tr>
          <tr>
            <td>'.gettext("URL").'</td>
            <td><input size="30" maxlength="255" name="url"></td>
          </tr>
          <tr>
            <td>'.gettext("Signature").'</td>
            <td><input size="30" maxlength="1024" name="signature"></td>
          </tr>
          <tr>
            <td>'.gettext("Share").'</td>
            <td align="center"><input size="2" maxlength="3" name="share" value="50"></td>
          </tr>
          <tr>
          <td align="center" colspan="2"><input name="submit" type="submit"></td>
          </tr>
        </tbody>
    </table>
          <input name="authenticator" type="hidden">
    </form>
    ');
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
        $query = 'share='.$values['share'];
        if ($values['detach']) $query = $query . ', detach=1';
        else $query = $query . ', detach=0';
        if ($values['update']) $query = $query . ', `update`=1';
        else $query = $query . ', `update`=0';
        $obj = $this->db->lookup_id($id,$this->table,$this->class);
        return($this->db->update($obj,$this->table,$query));
      }

      function delete($id)
      {
        $obj = $this->db->lookup_id($id,$this->table,$this->class);
        return($this->db->delete($obj,$this->table));
      }

      function insert($values)
      {
          $insert=True;
          if (empty($values['name'])) $insert=False;
          if (empty($values['url'])) $insert=False;
          if (empty($values['signature'])) $insert=False;
          if (empty($values['authenticator'])) $insert=False;
          if ($insert)
          {
            $query = '(name,url,signature,authenticator,share) values ("'
              .$values['name'].'", "'.$values['url'].'","'.str_replace(' ','',$values['signature']).'","'.$values['authenticator'].'",'.$values['share'].')';
            return($this->db->insert($this->table, $query));
          }
          else
              return(False);
      }
}
?>
