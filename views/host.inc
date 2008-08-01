<?php 
require_once("inc/html.inc");
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
        printf('<table border=1 align=center cellpadding="4">');
        if ($this->role == "root")
            printf('<tr><td>'.gettext("Host").'</td><td align="center">'.gettext("Pool").'</td><td
                align="center">'.gettext("Supplier").'</td>');
        else
            printf('<tr><td>'.gettext("Host").'</td><td align="center">'.gettext("Pool").'</td>');
        $row = count($obj);
        for ($i=0 ; $i<$row;$i++)
        {
            printf ("<tr>");
            printf ('<td align=center><a href=index.php?action=ed_host&id='.$obj[$i]->id.'>'.$obj[$i]->id.'</a></td>');
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
        printf('<form method="post" action="index.php?action=up_host&id='.$obj->id.'" name="host">');
        printf('<table border=1 align=center cellpadding="4">');
        printf('<tr><td>'.gettext("Host").'</td><td align="center">'.gettext("Pool").'</td><td>'.gettext("Delete").'</td>');
        printf ("<tr>");
        printf ('<td align=center><a href=index.php?action=ed_host&id='.$obj->id.'>'.$obj->id.'</a></td>');
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
        printf('<td align="center"><input name="delete" type=checkbox>');
        printf ("\n</tr>");
        printf('<td align="center" colspan="3"><input name="submit" type="submit"</td>');
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
