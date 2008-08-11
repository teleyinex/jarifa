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
require_once("inc/stats.inc");
require_once("model/data.inc");

class HTMLhost extends html
{
    public $role;
    public $supplier;
    public $db;
    public $pool;
    public $table='host';
    public $class='host';
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
        $this->load_config();
    }

    function page($view=null,$values=null)
    {
        switch ($view)
        {
            case 'edit':
            {
                $this->header(gettext("Hosts"));
                $this->menu($this->role,gettext("Editing host"));
                $obj = $this->get($values['id']);
                $pools = $this->pools();
                $this->html_edit($obj,$pools);
                break;
            }

            default:
            {
                $this->header(gettext("Wellcome").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Available Hosts"));
                break;
            }

        }

    }

    function table($obj,$pools)
    {
        $assigned_pool = False;
        printf('<table border=1 align=center cellpadding="4" class="form">');
        if ($this->role == "root")
            printf('<tr class="Hd"><td>'.gettext("Host").'</td><td align="center">'.gettext("Pool").'</td><td
                align="center">'.gettext("Supplier").'</td>');
        else
            printf('<tr class="Hd"><td>'.gettext("Host").'</td><td align="center">'.gettext("Pool").'</td>');
        $row = count($obj);
        for ($i=0 ; $i<$row;$i++)
        {
            printf ("<tr>");
            printf ('<td align=center><a href=index.php?action=ed_host&id='.$obj[$i]->id.'><img src="img/computer.png"
                border="0"></a></td>');
            for ($j=0;$j<count($pools);$j++)
            {
                if ($obj[$i]->poolid == $pools[$j]->id)
                {
                    printf('<td align="center"><a href=index.php?action=ed_pool&id='.$pools[$j]->id.'>'.$pools[$j]->name.'</td>');
                    $assigned_pool = True;
                }
            }
            if (!$assigned_pool) printf('<td align="center">'.gettext("Without Pool").'</td>');
            if ($this->role == "root") printf('<td align="center">'.$obj[$i]->supplier.'</td>');
            printf ("\n</tr>");
        }
        printf("</table>");
        $this->footer();
    }

    function html_edit($obj,$pools)
    {
        $st = new stats();
        printf('<form method="post" action="index.php?action=up_host&id='.$obj->id.'" name="host">');
        printf('<table border=1 align=center cellpadding="4" class="form">');
        printf('<tr class="Hd"><td><img src="img/computer.png"></td><td>'.gettext("Features").'</td></tr>');
        printf('<tr><td>'.gettext("Supplier").'</td>');
        printf('<td align="center"><select name="pool">');
        $row = count($pools);
        for ($i=0; $i<$row; $i++)
        {
            if ($this->role == "root")
                printf ('<option value="'.$pools[$i]->id.'">'.$pools[$i]->name.'->'.$pools[$i]->supplier.'</option>');
            else
                printf ('<option value="'.$pools[$i]->id.'">'.$pools[$i]->name.'</option>');
        }
        printf("</selection></td>");
        printf('<tr><td>'.gettext("Venue").'</td>');
        printf('<td>'.$obj->venue.'</td></tr>');
        printf('<tr><td>'.gettext("Number of CPUS").'</td>');
        printf('<td>'.$obj->p_ncpus.'</td></tr>');
        printf('<tr><td>'.gettext("Processor Vendor").'</td>');
        printf('<td>'.$obj->p_vendor.'</td></tr>');
        printf('<tr><td>'.gettext("Processor Model").'</td>');
        printf('<td>'.$obj->p_model.'</td></tr>');
        printf('<tr><td>'.gettext("FLOPS").'</td>');
        printf('<td>'.$obj->p_fpops.'</td></tr>');
        printf('<tr><td>'.gettext("IOPS").'</td>');
        printf('<td>'.$obj->p_iops.'</td></tr>');
        printf('<tr><td>'.gettext("OS").'</td>');
        printf('<td>'.$obj->os_name.'</td></tr>');
        $credit = $st->get_credit_host($obj);
        if (!empty($credit))
        {
            printf('<tr><td>'.gettext("Total Credit").'</td>');
            printf('<td>'.$credit->total_credit.'</td>');
            printf('<tr><td>'.gettext("RAC").'</td>');
            printf('<td>'.$credit->expavg_credit.'</td>');
        }
        printf('<tr><td>'.gettext("OS Version").'</td>');
        printf('<td>'.$obj->os_version.'</td></tr>');
        printf('<tr><td>'.gettext("Delete").'</td>');
        printf('<td align="center"><input name="delete" type=checkbox>');
        printf ("\n</tr>");
        printf('<td align="center" colspan="2"><input name="submit" type="submit"></td>');
        printf('</table>');
        printf("</form>");
        $this->footer();

    }


    function lists()
      {
          if ($this->role == 'root')
            $obj = $this->db->enum_fields($this->table,$this->class,'*','1', 'order by id');
          else
            $obj = $this->db->enum_fields($this->table,$this->class,'*','supplier="'.$this->supplier.'"', 'order by id');
          return($obj);
      }

    function pools()
    {
        if ($this->role == 'root')
            $obj = $this->db->enum_fields('pool','pool','*','1','order by id');
        else
            $obj = $this->db->enum_fields('pool','pool','*','supplier="'.$this->supplier.'"','order by id');
        return($obj);
    }


      function get($id)
      {
          return ($this->db->lookup_fields($this->table,$this->class,'*','id="'.$id.'"'));
      }

      function update($id,$values)
      {
        $obj = $this->db->lookup_id($id,$this->table,$this->class);
        $query = 'poolid='.$values['pool'];
        return($this->db->update($obj,$this->table,$query));
      }

      function delete($id)
      {
        $obj = $this->db->lookup_id($id,$this->table,$this->class);
        return($this->db->delete($obj,$this->table));
      }

}
?>
