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
                $this->header(gettext("Pools"));
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
                ');
            if ($obj[$i]->user_active==1) printf('<td align=center>Yes</td>');
            else printf('<td align=center>No</td>');
            printf('
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
        printf('<form method="post" action="index.php?action=up_pool&id='.$obj->id.'" name="pool">');
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
        else printf('<td><input name="user_active" type="checkbox" ></td>');
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
                  <td>'.gettext("Delete Pool").'</td>
                  <td><input name="delete" type=checkbox></td>
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
    printf('<form method="post" action="index.php?action=in_pool">');
        printf('
            <table align="center" style="text-align: left; width: 400px;" border="1"
             cellpadding="2" cellspacing="2">
              <tbody>
                <tr>
                  <td>'.gettext("Name").'</td>
                  <td><input name="name" size="10"></td>
                </tr>
                <tr>
                  <td>'.gettext("Run BOINC Always").'</td>
                  ');
        printf('<td><input name="user_active" type="checkbox" checked ></td>');
        printf('
                </tr>
                <tr>
                  <td>'.gettext("Start Hour").'</td>
                  <td><input name="start_hour" size="6" value="00:00:00"></td>
                </tr>
                <tr>
                  <td>'.gettext("End Hour").'</td>
                  <td><input name="end_hour" size="6" value="00:00:00"></td>
                </tr>
                <tr>
                  <td>'.gettext("Net Upload").'</td>
                  <td><input name="net_upload" size="2" value="0"></td>
                </tr>
                <tr>
                  <td>'.gettext("Net Download").'</td>
                  <td><input name="net_download" size="2"  value="0"></td>
                </tr>
                <tr>
                  <td>'.gettext("HD Max Space employed").'</td>
                  <td><input name="hd_max" size="2" value="10"></td>
                </tr>
                <tr>
                  <td>'.gettext("Minimum HD Space Left").'</td>
                  <td><input name="hd_left" size="2" value="0.1"></td>
                </tr>
                <tr>
                  <td>'.gettext("Max RAM when Active").'</td>
                  <td><input name="ram_use_max" size="2" value="50"></td>
                </tr>
                <tr>
                  <td>'.gettext("Max RAM when Idle").'</td>
                  <td><input name="ram_idle_max" size="2" value="90"></td>
                </tr>
                <tr>
                  <td align="center" colspan="2" rowspan="1"><input type="submit"></td>
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

      function update($id,$values)
      {
        $obj = $this->db->lookup_id($id,$this->table,$this->class);
        $query = 'name="'.$values['name'].'",';
        print_r($values['user_active']);
        if ($values['user_active']=="on") $query = $query . ' user_active=1,';
        else $query = $query . ' user_active=0,';
        $query = $query . ' start_hour="' . $values['start_hour'].
            '", end_hour="'   . $values['end_hour'].
            '", net_upload=' . $values['net_upload'].
            ', net_download=' . $values['net_download'].
            ', hd_max=' . $values['hd_max'].
            ', hd_left='. $values['hd_left'].
            ', ram_use_max='. $values['ram_use_max']. 
            ', ram_idle_max='. $values['ram_idle_max'];
        return($this->db->update($obj,$this->table,$query));
      }

      function delete($id)
      {
        $obj = $this->db->lookup_id($id,$this->table,$this->class);
        return($this->db->delete($obj,$this->table));
      }

      function insert($values)
      {
          print_r($values);
          $insert=True;
          if (empty($values['name'])) $insert=False;
          if ($values['user_active']=="on") $values['user_active']=1;
          else $values['user_active']=0;
          if (empty($values['start_hour'])) $insert=False;
          if (empty($values['end_hour'])) $insert=False;
          //if (empty($values['net_upload'])) $insert=False;
          //if (empty($values['net_download'])) $insert=False;
          if (empty($values['hd_max'])) $insert=False;
          if (empty($values['hd_left'])) $insert=False;
          if (empty($values['ram_use_max'])) $insert=False;
          if (empty($values['ram_idle_max'])) $insert=False;
          if ($insert)
          {
            $query = '(name,user_active, start_hour, end_hour, net_upload, net_download, hd_max, hd_left, ram_use_max,
                ram_idle_max) values ("'
              .$values['name'].'",
              '.$values['user_active'].',"'.$values['start_hour'].'","'.$values['end_hour'].'",'.$values['net_upload'].',
              '.$values['net_download'].','.$values['hd_max'].','.$values['hd_left'].', "'.$values['ram_use_max'].'",
              "'.$values['ram_idle_max'].'")';
            return($this->db->insert($this->table, $query));
          }
          else
              return(False);
      }
}
?>
