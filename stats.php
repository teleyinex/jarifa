<?php 
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
