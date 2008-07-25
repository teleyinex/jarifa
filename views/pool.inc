<?php 
require_once("inc/html.inc");

class HTMLpool extends html
{
    public $role;
    public $db;
    public $supplier;
    public $table='pool';
    public $class='pool';
    public $language;

    function __construct($language,$role=null,$supplier=null,$db)
    {
        $this->language=$language;
        putenv("LANG=$this->language");
        setlocale(LC_ALL, $this->language);
        $domain = 'text';
        bindtextdomain($domain, "lang");
        textdomain($domain);
        $this->role = $role;
        $this->supplier = $supplier;
        $this->db = $db;
        $this->language;
    }

    function page($view=null,$values=null)
    {
        switch ($view)
        {
            case 'edit':
            {
                $this->header(gettext("Pools"));
                $this->menu($this->role,gettext("Editing pool"));
                $obj = $this->get($values['id']);
                $this->html_edit($obj);
                break;
            }

            case 'insert':
            {
                $this->header(gettext("Projects"));
                $this->menu($this->role,gettext("Create a new pool"));
                $this->html_insert(); 
                break;
            }
            default:
            {
                $this->header(gettext("Wellcome").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Available Pools"));
                break;
            }

        }

    }

    function table($obj)
    {
        printf('<table border=1 align=center cellpadding="4">');
        printf('<tr><td colspan=2></td><td align="center" colspan=2>'.gettext("Time").'<td align="center" colspan=2>'.gettext("Network").'
                <td align="center" colspan=2>'.gettext("Hard Disk Usage").'</td><td align="center"
                colspan=2>'.gettext("Memory Usage").'</td></tr>');
        printf('<tr><td align="center">'.gettext("Name").'</td><td align="center">'.gettext("Run Always").'</td>
            <td align="center">'.gettext("Start").'</td><td align="center">'.gettext("End").'</td>
            <td align="center">'.gettext("Upload").'</td><td align="center">'.gettext("Download").'</td>
            <td align="center">'.gettext("Max Space").'</td><td align="center">'.gettext("Left Space").'</td>
            <td align="center">'.gettext("Max Active").'</td><td align="center">'.gettext("Max Idle").'</td>
            </tr>');
        $row = count($obj);
        for ($i=0 ; $i<$row;$i++)
        {
            printf ("<tr>");
            printf ('<td align=center><a href=index.php?action=ed_pool&id='.$obj[$i]->id.'>'.$obj[$i]->name.'</a></td>
                <td align=center>'.$obj[$i]->user_active.'</td>
                <td align="center">'.$obj[$i]->start_hour.'</td>
                <td align="center">'.$obj[$i]->end_hour.'</td>
                <td align="center">'.$obj[$i]->net_upload.' KB/s</td>
                <td align="center">'.$obj[$i]->net_download.' KB/s</td>
                <td align="center">'.$obj[$i]->hd_max.' GB</td>
                <td align="center">'.$obj[$i]->hd_left.' GB</td>
                <td align="center">'.$obj[$i]->ram_use_max.' %%</td>
                <td align="center">'.$obj[$i]->ram_idle_max.' %%</td>'
                  );
            printf ("\n</tr>");
        }
        printf("</table>");
        printf('<div style="text-align: center;"><a href=index.php?action=vin_pool>'.gettext("New Pool").'</a></div>');
        $this->footer();
    }

    function html_edit($obj)
    {
        printf('<form method="post" action="index.php?action=up_pool&id='.$obj->id.'" name="project">');
        printf('
            <table align="center" style="text-align: left; width: 400px;" border="1"
             cellpadding="2" cellspacing="2">
              <tbody>
                <tr>
                  <td>'.gettext("Name").'</td>
                  <td><input name="name" size="10" value="'.$obj->name.'"></td>
                </tr>
                <tr>
                  <td>'.gettext("Run BOINC Always").'</td>
                  ');
        if ($obj->user_active==1) printf('<td><input name="user_active" type="checkbox" checked></td>');
        else printf('<td><input name="user_active" value="false" type="checkbox"></td>');
        printf('
                </tr>
                <tr>
                  <td>'.gettext("Start Hour").'</td>
                  <td><input name="start_hour" size="6" value="'.$obj->start_hour.'"></td>
                </tr>
                <tr>
                  <td>'.gettext("End Hour").'</td>
                  <td><input name="end_hour" size="6" value="'.$obj->end_hour.'"></td>
                </tr>
                <tr>
                  <td>'.gettext("Net Upload").'</td>
                  <td><input name="net_upload" size="2" value="'.$obj->net_upload.'"></td>
                </tr>
                <tr>
                  <td>'.gettext("Net Download").'</td>
                  <td><input name="net_download" size="2"  value="'.$obj->net_download.'"></td>
                </tr>
                <tr>
                  <td>'.gettext("HD Max Space employed").'</td>
                  <td><input name="hd_max" size="2" value="'.$obj->hd_max.'"></td>
                </tr>
                <tr>
                  <td>'.gettext("Minimum HD Space Left").'</td>
                  <td><input name="hd_left" size="2" value="'.$obj->hd_left.'"></td>
                </tr>
                <tr>
                  <td>'.gettext("Max RAM when Active").'</td>
                  <td><input name="ram_use_max" size="2" value="'.$obj->ram_use_max.'"></td>
                </tr>
                <tr>
                  <td>'.gettext("Max RAM when Idle").'</td>
                  <td><input name="ram_idle_max" size="2" value="'.$obj->ram_idle_max.'"></td>
                </tr>
                <tr>
                  <td align="center" colspan="2" rowspan="1"><input type="submit"></td>
                </tr>
              </tbody>
            </table>
            ');
        printf("</form>");
        $this->footer();

    }

    function html_insert()
    {
    printf('
    <form method="post" action="index.php?action=in_pool">
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
