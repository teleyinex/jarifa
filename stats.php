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

require_once("inc/stats.inc");

$st = new stats();

$projects = $st->get_projects();
foreach ($projects as $project)
{
    $authenticator = $st->get_authenticator($project->url);
    if (!empty($authenticator))
        {
            $xml = $st->get_stats($project->url,$authenticator);
            $st->insert_user_stats($xml,$project->name);
            foreach ($xml->host as $host)
            {
                $st->insert_host_stats($host,$xml->host_cpid,$project->name);
            }
        }
    
}

$st->draw_credit_per_day_user($projects);
$st->draw_gflops_per_day_user($projects);
$st->draw_credit_per_day_supplier($projects);
$st->draw_gflops_per_day_supplier($projects);
?>
