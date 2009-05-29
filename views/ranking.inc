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

class HTMLranking extends html
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
        $this->role = "volunteer";
        $this->db = $db;
        $this->db->do_query("SET NAMES 'utf8'");
        $this->load_config();
    }

    function page($view=null,$values=null)
    {
        switch ($view)
        {
            case 'supplier':
            {
                $this->header(gettext("Ranking of suppliers"));
                $this->html_ranking_suppliers();
                break;
            }

            default:
            {
                $this->header(gettext("Ranking of volunteers"));
                $this->html_ranking_volunteers();
                break;
            }

        }

    }

    function table($obj)
    {
        printf('<table border=1 align=center cellpadding="4" class="form">');
        printf('<tr class="Hd"><td align="center">'.gettext("Volunteer").'</td><td align="center">'.gettext("Credit").'</tr>');
        $row = count($obj);
        for ($i=0 ; $i<$row;$i++)
        {
            printf ("<tr>");
            printf ('<td align=center>'.$obj[$i]->name.'</td>
                <td align="center">'.$obj[$i]->credit.'</td>');
            printf ("\n</tr>");
        }
        printf("</table>");
        $this->footer();
    }


    function lists()
      {
          $obj = $this->db->enum_fields('host','host','*','supplier="Volunteer"', 'group by username');
          foreach ($obj as $host)
          {
               echo $host->username; 
          }
          return($obj);
      }

      function get($id)
      {
          return ($this->db->lookup_fields($this->table,$this->class,'*','id="'.$id.'"'));
      }

      function get_projects()
      {
        return($this->db->enum_fields('project','project','*','1','order by name'));
      }

      function get_volunteers($userid=null)
      {
          if ($userid==null)
            return($this->db->enum_fields($this->table,$this->class,'*','supplier="Volunteer"','group by name'));
          else
            return($this->db->enum_fields($this->table,$this->class,'*','supplier="Volunteer" and name="'.$userid.'"','group by name'));
      }

      function get_user_host_cpid($username)
      {
          return($this->db->enum_fields('host','host','*','username="'.$username.'"','group by host_cpid'));
      }

      function get_total_credit($host,$n)
      {
          return($this->db->enum_fields('stats_host','stats_host','*','host_cpid="'.$host.'"','order by date desc limit '.$n));
      }

      function obtain_ranking_volunteers($userid=null)
      {
          $ranking = array();
          $n = count($this->get_projects());
          if ($userid==null)
              $volunteers = $this->get_volunteers();
          else
              $volunteers = $this->get_volunteers($userid);
          foreach ($volunteers as $volunteer)
          {
                $ranking[$volunteer->name]= 0; 
                $hosts = $this->get_user_host_cpid($volunteer->name);
                foreach ($hosts as $host)
                {
                    $stats = $this->get_total_credit($host->host_cpid,$n); 
                    $credit = 0;
                    foreach ($stats as $item) 
                    {
                        $credit += $item->total_credit;
                    }
                    $ranking[$volunteer->name]= $ranking[$volunteer->name] + $credit;
                }
          
          } 
          arsort($ranking);
          //return(array_slice($ranking,0,10));
          return($ranking);
      }

      function table_ranking_volunteers($userid=null)
      {
           $ranking = $this->obtain_ranking_volunteers();
          printf("<table border=1 cellpadding=4 class=form><tr><td>".gettext("Position")."</td><td>".gettext("User")."</td><td>".gettext("Total credit")."</td></tr>");
          $position = 1;
          $total = count($ranking);
          if ($userid == null)
          {
            // We only want to show the 10 best volunteers
            $ranking = array_slice($ranking,0,10);
            foreach($ranking as $key => $val)
            {
                if ($val != 0)
                {
                      printf("<tr><td align=center>$position</td><td>$key</td><td>$val</td></tr>\n");
                      $position += 1;
                }
            }
          }
          else
          {
              foreach($ranking as $key =>$val)
                  if ($key!=$userid) $position+=1;
                  else break;
              printf("<tr><td align=center>$position/$total</td><td>$userid</td><td>$ranking[$userid]</td></tr>\n");
          }

          printf("</table>");
     
      }

      function html_ranking_volunteers($userid=null)
      {

          printf("<div id=content align=center>");
          $this->table_ranking_volunteers();
          $this->footer();
      }

}
?>
