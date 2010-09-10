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

require_once("inc/rpc.inc");

$rpc = new rpc;

$host = simplexml_load_file("php://input");

if ($rpc->auth($host))
{
    //fill reply with name and public key
    $rpc->xmlSigningKey();
    //check, if host already exists
    if (!$rpc->exist_host())
    {
        //host is unknown, add it 
        $id = $rpc->add_host();
        if ($id == False)
        {
            $rpc->error('database operation (add host) failed');
            echo $rpc->dom->saveXML();
            return;         
        }
    }   
    //  
    $rpc->xmlRepeat_sec();
    //fill reply withh preferences from pool
    $rpc->xmlPreferences();
    //
    $rpc->xmlOpaqueID();
    //fill reply with project infos
    $projects = $rpc->projects();
    foreach ($projects as $project)
    {
        $rpc->xmlProject($project);
    }

    $rpc->xmlRssFeed();
    //return the reply as xml string
    echo $rpc->dom->saveXML();
}
else {
    $rpc->xmlSigningKey();
    $rpc->error('host authentication failed');  
    echo $rpc->dom->saveXML();
}
?>
