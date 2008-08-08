<?php 
require_once("inc/html.inc");

class HTMLstat extends html
{
    public $role;
    public $db;
    public $table='project';
    public $class='project';
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
        $this->load_config();
    }

    function page($view=null,$values=null)
    {
        switch ($view)
        {
            case 'credit':
            {
                $this->header(gettext("Statistics").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Total Credit"));
                $this->figures('credit');
                $this->footer();
                break;
            }

            case 'gflops':
            {
                $this->header(gettext("Statistics").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("GFLOPS"));
                $this->figures('gflops');
                $this->footer();
                break;
            }

            default:
            {
                $this->header(gettext("Statistics").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Figures"));
                $this->options();
                $this->footer();
                break;
            }

        }

    }

    function projects()
    {
        $projects = $this->db->enum_fields("project","project","*","1","group by id");
        if (!empty($projects)) return ($projects);
        else return (False);
    }

    function suppliers()
    {
        $supplier = $this->db->lookup_fields("user","user","supplier","1","group by supplier");
        if (!empty($suppliers)) return ($suppliers);
        else return (False);
    }

    function options()
    {
        printf('
                <div id="stats">
                <table class="stats">
                  <tbody>
                    <tr>
                      <td><a href=index.php?action=st_credit><img alt="Credit" src="img/credit.png" border="0"></a></td>
                      <td><a href=index.php?action=st_gflops><img src="img/gflops.png" border="0" alt="GFLOPS"></td>
                    </tr>
                  </tbody>
                </table>
                </div>
            ');
    
    }

    function figures($type=null)
    {
        $projects = $this->projects();

        printf('<div id="stats">');
        foreach($projects as $project)
        {
            if ($this->role == "root") 
            {
                printf("<img src=img/stats/".$type."_".$project->name.".png><br>");
            
            }
            else
            {
                printf("<img src=img/stats/".$type."_".$project->name."_".$this->supplier.".png><br>");
            
            }
        
        }
        printf("</div>");
    
    }


}
?>
