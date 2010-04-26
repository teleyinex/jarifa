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

require_once("inc/poll.inc");
require_once("inc/identica.inc");

$poll = new poll();

$two_projects = array();

// Attach to clients only the two most voted projects (in case of a draft with several projects, choose one random)
$projects = $poll->get_projects($order="votes");

// Save stats in stats_poll table

$poll->save_stats();

// Rank of votes, zero position has the highest number of votes and last position the lowest one.
$votes = $poll->get_votes();

if (count($votes)>1) // If there are more than two different scores of votes
{
    for ($i=0;$i<2;$i++)
        {
            $clause = "votes=".$votes[$i]->votes;
            $candidate_projects= $poll->get_projects("name",$clause);
            // If there are more than one project with the same number of votes, we choose one of them randomly
            shuffle($candidate_projects);
            $two_projects[] = $candidate_projects[0];
        }
}
else 
{
    $clause = "votes=".$votes[0]->votes;
    $candidate_projects = $poll->get_projects("name",$clause);
    // If there are more than one project with the same number of votes, we choose one of them randomly
    shuffle($candidate_projects);
    $two_projects[] = $candidate_projects[0];
    $two_projects[] = $candidate_projects[1];
}

// Enable the two most voted projects

// First, dettach all the projects
$projects = $poll->get_projects();
$poll->disable_projects($projects);
// Then attach only the two most voted
$poll->enable_projects($two_projects);

// If identica is enabled, post the two most voted projects to Identi.ca
if ($poll->conf->xpath("/conf/identica"))
{
    $Identica = new identica($poll->conf->account_manager->language);
    $Identica->update_status(gettext("We are now collaborating with the following research projects: #").strtolower(str_replace("@","at",$two_projects[0]->name)).gettext(" and #").strtolower(str_replace("@","at",$two_projects[1]->name)));

}

// Reset the number of votes that each project has received, and allow users to vote again
$poll->reset_votes();

?>
