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

require_once("inc/db_conn.inc");
require_once("views/project.inc");
require_once("model/data.inc");
include("libchart-1.2/libchart/classes/libchart.php");

define("CREDIT_HALF_LIFE", 86400*7);

class stats extends DBConn
{
      public $db_user;
      public $db_password;
      public $db_host;
      public $db_name;
      public $db=''; // The DB handler
      public $boinc_user_email;
      public $boinc_user_name;
      public $boinc_passwd;
      function __construct()
      {
          $this->load_config();
          $this->db = new DbConn();
          $this->db->init_conn($this->db_user,$this->db_password,$this->db_host,$this->db_name);
      }

      function load_config()
      {
          $conf = simplexml_load_file("conf/ogm.conf");
          $this->db_user = $conf->db->user;
          $this->db_password = $conf->db->password;
          $this->db_host = $conf->db->host;
          $this->db_name = $conf->db->name;
          $this->boinc_user_email = $conf->account_manager->boinc_user_email;
          $this->boinc_user_name = $conf->account_manager->boinc_user_name;
          $this->boinc_passwd = $conf->account_manager->boinc_passwd;
      }

      function get_authenticator($url)
      {
          // Web RPC for getting a boinc account 
          $request='?email_addr='.$this->boinc_user_email.'&passwd_hash='.md5($this->boinc_passwd.$this->boinc_user_email);
          $authenticator = file_get_contents(trim($url).'/lookup_account.php'.$request, false);
          $p = xml_parser_create();
          xml_parse_into_struct($p, $authenticator, $vals, $index);
          xml_parser_free($p);
          $authenticator =  $vals[$index['AUTHENTICATOR'][0]]['value'];
          return($authenticator);
      }

      function get_projects()
      {
          $projects = $this->db->enum_fields('project','project','*','1','order by id');
          if (!empty($projects)) return($projects);
          else return(False);
      }

      function get_stats($url,$authenticator)
      {
          $request='?auth='.$authenticator.'&format=xml';
          $stats = file_get_contents(trim($url).'/show_user.php'.$request, false);
          $xml = new SimpleXMLElement($stats);
          return($xml);
      }


      function decay_average($avg, $avg_time, $now = 0) 
      {
          $M_LN2 = 0.693147180559945309417;
          $credit_half_life = 86400 * 7;
          if ($now == 0) 
          {
              $now = time();
          }
          $diff = $now - $avg_time;
          $weight = exp(-$diff * $M_LN2/$credit_half_life);
          $avg *= $weight;
          return $avg;
      }

      function insert_user_stats($user,$project)
      {
          $query = '(project, total_credit, expavg_credit, expavg_time) values ';
          $query = $query.'("'.$project.'",'." $user->total_credit, $user->expavg_credit, $user->expavg_time)";
          $out = $this->db->insert('stats_user',$query);
          if (!empty($out)) return(True);
          else return(False);
      }

      function insert_host_stats($host,$cpid,$project)
      {
          $db_host = $this->db->lookup_fields("host","host","*",'host_cpid="'.$host->host_cpid.'"');
          if (!empty($db_host))
          {
              $query = '(id, host_cpid, supplier, project, total_credit, expavg_credit, expavg_time) values (';
              $query = $query .$host->id.',"'.$host->host_cpid.'", "'
                              .$db_host->supplier.'","'.$project.'",'
                              .$host->total_credit.','.$host->expavg_credit.','
                              .$host->expavg_time.')';
              $out = $this->db->insert('stats_host',$query);
              if (!empty($out))
              {
                  $query2 = 'p_ncpus='.$host->p_ncpus.', p_vendor="'.$host->p_vendor.'", p_model="'.$host->p_model.'", p_fpops='.$host->p_fpops.
                  ', p_iops='.$host->p_iops.', os_name="'.$host->os_name.'", os_version="'.$host->os_version.'"';
                  $out2 = $this->db->update($db_host,"host",$query2);
              }
              if (!empty($out2)) return(True);
              else return(False);
          }
          else return(False);
      }

      function get_days($table,$class)
      {
          $out = $this->db->enum_fields($table,$class,'date','1','group by date');
          $days = array();
          foreach ($out as $value)
          {
              $days[] = $value->date;
          } 
          return($days);
      }

      function get_total_credit($table,$class,$clause=null)
      {
          if ($clause == null) $out = $this->db->enum_fields($table,$class,'*','1','group by date');
          else $out = $this->db->enum_fields($table,$class,'*',$clause,'group by date');
          $credit = array();
          foreach ($out as $value)
          {
              $credit[$value->date]=$value;
            
          }
          return($credit);
      }

      function draw_credit_per_day_user($projects)
      {
          $chart = new LineChart();
          $dataset = new XYSeriesDataSet();
          $data = array();
          $credit = $this->get_total_credit('stats_user','stats_user');
          $days = array_keys($credit);
          foreach ($projects as $project)
          {
                $serie = new XYDataSet();
                foreach ($days as $value)
                {
                    list($day,$hour) = split(" ", $value);
                    if ($project->name == $credit[$value]->project)
                    {
                        $serie->addPoint(new Point($day,$credit[$value]->total_credit));
                    }
                }
                $chart->setDataSet($serie);
                $chart->setTitle("Total Credit for project ". $project->name);
                $chart->getPlot()->setGraphCaptionRatio(0.62);
                $chart->render("img/stats/credit_$project->name.png");
          }

      }

      function draw_gflops_per_day_user($projects)
      {
          $chart = new LineChart();
          $dataset = new XYSeriesDataSet();
          $data = array();
          $credit = $this->get_total_credit('stats_user','stats_user');
          $days = array_keys($credit);
          foreach ($projects as $project)
          {
                $serie = new XYDataSet();
                foreach ($days as $value)
                {
                    list($day,$hour) = split(" ", $value);
                    if ($project->name == $credit[$value]->project)
                    {
                        $gflops = $this->decay_average($credit[$value]->expavg_credit,$credit[$value]->expavg_time,0);
                        $serie->addPoint(new Point($day,$gflops));
                    }
                }
                $chart->setDataSet($serie);
                $chart->setTitle("Total GFLOPS for project ". $project->name);
                $chart->getPlot()->setGraphCaptionRatio(0.62);
                $chart->render("img/stats/gflops_$project->name.png");
          }

      }
      
      function draw_gflops_per_day_supplier($projects)
      {
          $suppliers = $this->db->enum_fields("user","user","supplier","1","group by supplier");
          foreach ($projects as $project)
          {
                $chart = new LineChart();
                $dataset = new XYSeriesDataSet();
                foreach ($suppliers as $sup)
                {
                    $credit = $this->get_total_credit('stats_host','stats_host','supplier="'.$sup->supplier.'"');
                    $days = array_keys($credit);

                    $serie = new XYDataSet();
                    foreach ($days as $value)
                    {
                        list($day,$hour) = split(" ", $value);
                        if ($project->name == $credit[$value]->project) 
                        {
                            $gflops = $this->decay_average($credit[$value]->expavg_credit,$credit[$value]->expavg_time,0);
                            $serie->addPoint(new Point($day,$gflops));
                        }
                    }
                    $chart->setDataSet($serie);
                    $chart->setTitle("Total GFLOPS for project ". $project->name);
                    $chart->getPlot()->setGraphCaptionRatio(0.62);
                    $chart->render("img/stats/gflops_".$project->name."_".$sup->supplier.".png");
                }
          }

      }

      function draw_credit_per_day_supplier($projects)
      {
          $suppliers = $this->db->enum_fields("user","user","supplier","1","group by supplier");
          foreach ($projects as $project)
          {
                $chart = new LineChart();
                $dataset = new XYSeriesDataSet();
                foreach ($suppliers as $sup)
                {
                    $credit = $this->get_total_credit('stats_host','stats_host','supplier="'.$sup->supplier.'"');
                    $days = array_keys($credit);

                    $serie = new XYDataSet();
                    foreach ($days as $value)
                    {
                        list($day,$hour) = split(" ", $value);
                        if ($project->name == $credit[$value]->project) 
                        {
                            $serie->addPoint(new Point($day,$credit[$value]->total_credit));
                        }
                    }
                    $chart->setDataSet($serie);
                    $chart->setTitle("Total Credit for project ". $project->name);
                    $chart->getPlot()->setGraphCaptionRatio(0.62);
                    $chart->render("img/stats/credit_".$project->name."_".$sup->supplier.".png");
                }
          }

      }

}
?>
