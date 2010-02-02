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

require_once("inc/db_conn.inc");
require_once("model/data.inc");
include("libchart-1.2/libchart/classes/libchart.php");

class poll extends DBConn
{
      public $db_user;
      public $db_password;
      public $db_host;
      public $db_name;
      public $db=''; // The DB handler
      public $boinc_user_email;
      public $boinc_user_name;
      public $boinc_passwd;
      public $context = False;
      public $proxy_name;
      public $proxy_port;

      function __construct()
      {
          $this->load_config();
          $this->db = new DbConn();
          $this->db->init_conn($this->db_user,$this->db_password,$this->db_host,$this->db_name);
      }

      function load_config()
      {
          $conf = simplexml_load_file("conf/jarifa.conf");
          $this->db_user = $conf->db->user;
          $this->db_password = $conf->db->password;
          $this->db_host = $conf->db->host;
          $this->db_name = $conf->db->name;
          $this->boinc_user_email = $conf->account_manager->boinc_user_email;
          $this->boinc_user_name = $conf->account_manager->boinc_user_name;
          $this->boinc_passwd = $conf->account_manager->boinc_passwd;
          // Are we using a proxy?
          if ($conf->xpath("/conf/proxy"))
          {
            error_log("Using proxy configuration");
            $this->proxy = True;
            $this->proxy_name = $conf->proxy->name;
            $this->proxy_port = $conf->proxy->port;
            if ($conf->proxy->request_fulluri == 1) $this->proxy_request_fulluri = True;
            else $this->proxy_request_fulluri = False;
            $opts= array('http' => array('request_fulluri' => $this->proxy_request_fulluri,'proxy' =>'tcp://'.$this->proxy_name.':'.$this->proxy_port));
            $this->context = stream_context_create($opts);
          }
          else $this->context = null;
      }

      function get_projects($order="name",$clause="1")
      {
          if ($order=="name")
            $projects = $this->db->enum_fields('project','project','*',$clause,'order by name');
          else
            $projects = $this->db->enum_fields('project','project','*',$clause,'order by votes desc');
          if (!empty($projects)) return($projects);
          else return(False);
      }

      function get_votes()
      {
          return($this->db->enum_fields('project','project','*','1 group by votes','order by votes desc'));
      
      }

      function save_stats()
      {
          $projects = $this->get_projects();
          foreach($projects as $project)
          {
              $clause = '(project,`date`,votes) VALUES("'.$project->name.'",CURDATE(),'.$project->votes.')';
              $this->db->insert('stats_poll',$clause);
          }
      
      }

      function disable_projects($projects)
      {
          foreach ($projects as $project)
          {
              $this->db->update($project,'project','detach=1');
          }
      }

      function enable_projects($projects)
      {
          foreach ($projects as $project)
          {
              $this->db->update($project,'project','detach=0');
          }
      }

      function check_vote($userid)
      {
        $user = $this->db->lookup('user','user','name="'.$userid.'"');
        return ($user->voted);
      }

      function user_voted($userid,$project)
      {
          // Update user's voted field to true
          $this->db->update_aux('user','voted=1 where name="'.$userid.'"'); 
          // Give one more vote for the chosen project
          $this->db->update_aux('project','votes=votes+1 where name="'.$project.'"');
      }

      function reset_votes()
      {
          $projects = $this->get_projects();
          $users = $this->db->enum_fields('user','user','*','1','order by name');
          foreach ($projects as $project)
          {
              $this->db->update($project,'project','votes=0');
          }
          foreach ($users as $user)
          {
              $this->db->update($user,'user','voted=0');
          }
      }

      function draw_results()
      {
          $projects = $this->get_projects($order="votes");
          $chart = new VerticalBarChart();
          $dataSet = new XYDataSet();
          foreach ($projects as $project)
          {
              $dataSet->addPoint(new Point($project->name,$project->votes));
          
          }
          $chart->setDataSet($dataSet);
          // First, we check if there is a folder to write in the image
          if (!(is_dir("img/stats/")))
              mkdir("img/stats/");
          $chart->setTitle("");
          $chart->render("img/stats/votes.png");
      }

}
?>
