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
        $domain = 'messages';
        bindtextdomain($domain, "lang");
        textdomain($domain);
        $this->role = $role;
        $this->supplier = $supplier;
        $this->db = $db;
        $this->db->do_query("SET NAMES 'utf8'");
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
                $this->footer(true);
                break;
            }

            case 'gflops':
            {
                $this->header(gettext("Statistics").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("GFLOPS"));
                $this->figures('gflops');
                $this->footer(true);
                break;
            }

            case 'cpus':
            {
                $this->header(gettext("Statistics").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("CPUs"));
                $this->figures('cpus');
                $this->footer(true);
                break;
            }

            default:
            {
                $this->header(gettext("Statistics").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Figures"));
                $this->options();
                $this->footer(true);
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
                <div id="stats_index">
                <table class="stats">
                  <tbody>
                    <tr>
                      <td><a href="index.php?action=st_credit"><img src="img/credit.png" alt="credit" border="0"></a></td>
                      <td><a href="index.php?action=st_gflops"><img src="img/gflops.png" alt="GFLOPS" border="0"></a></td>
                    </tr>
                    <tr>
                      <td><a href="index.php?action=st_cpus"><img src="img/cpus.png" alt="CPUs" border="0"></a></td>
                  </tbody>
                </table>
                </div>
            ');
    
    }

    function figures($type=null)
    {
        printf('<div id="stats">'."\n");

        if ($type==null)
        {
            $projects = $this->projects();

            foreach($projects as $project)
            {
                if ($this->role == "root") 
                {
                    printf('<img src="img/stats/'.$type.'_'.$project->name.'.png" alt="'.$type.'_'.$project->name.'"><br>'."\n");
                
                }
                else
                {
                    printf('<img src="img/stats/'.$type.'_'.$project->name.'_'.$this->supplier.'.png"
                        alt="'.$type.'_'.$project->name.'_'.$this->supplier.'"><br>');
                
                }
            
            }
        }
        else
        {
            if ($this->role == "root")
                printf('<img src="img/stats/cpus.png" alt="CPUs"><br>');
            else
            {
                printf('<img src="img/stats/cpus_'.$this->supplier.'.png" alt="CPUs">');
            
            }
        
        }
        printf("</div>");
    
    }


}
?>
