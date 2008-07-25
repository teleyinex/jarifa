<?php 
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

    function html_edit($obj)
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

    function html_insert()
    {
    printf('
    <form method="post" action="index.php?action=in_project">
    <table style="text-align: left; width: 100px;" border="1"  cellpadding="2" cellspacing="2">
        <tbody>
          <tr>
            <td>Name</td>
            <td><input size="30" maxlength="255" name="name"></td>
          </tr>
          <tr>
            <td>URL</td>
            <td><input size="30" maxlength="255" name="url"></td>
          </tr>
          <tr>
            <td>Signature</td>
            <td><textarea cols="43" rows="5" name="signature"></textarea></td>
          </tr>
          <tr>
            <td>Authenticator</td>
            <td><input size="30" name="authenticator"></td>
          </tr>
          <tr>
            <td>Share</td>
            <td align="center"><input size="2" maxlength="3" name="share" value="50"></td>
          </tr>
          <tr>
          <td align="center" colspan="2"><input name="submit" type="submit"></td>
          </tr>
        </tbody>
    </table>
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

      function update($id,$share)
      {
        $obj = $this->db->lookup_id($id,$this->table,$this->class);
        return($this->db->update($obj,$this->table,'share = '.$share));
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
              .$values['name'].'", "'.$values['url'].'","'.$values['signature'].'","'.$values['authenticator'].'",'.$values['share'].')';
            return($this->db->insert($this->table, $query));
          }
          else
              return(False);
      }
}
?>
