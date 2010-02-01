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

$poll = new poll();

$two_projects = array();

// Attach to clients only the two most voted projects (in case of a draft with several projects, choose one random)
$projects = $poll->get_projects($order="votes");

// Rank of votes
$votes = $poll->get_votes();

for ($i=0;$i<2;$i++)
{
    if (count($votes)>1) // If there are more than two different scores of votes
    {
        $clause = "votes=".$votes[$i]->votes;
    }
    else // else
        $clause = "votes=".$votes[0]->votes;

    // Do not allow duplicates
    if ($i >=1)
        $clause = $clause . ' and name != "'.$two_projects[0]->name.'"';

    $projects = $poll->get_projects("name",$clause);
    shuffle($projects);
    $two_projects[] = $projects[0];
}

// Enable the two most voted projects

// First, dettach all projects
$poll->disable_projects($projects);
// Then attach only the two most voted
$poll->enable_projects($two_projects);

// Reset the number of votes that each project has received, and allow users to vote again
$poll->reset_votes();

?>
