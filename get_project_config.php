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
along with Jarifa.  If not, see <http://www.gnu.org/licenses/>.*/

$conf = simplexml_load_file("conf/jarifa.conf");

$dom = new DOMDocument('1.0', 'UTF-8');
$dom->formatOutput = true;
$root = $dom->createElement('project_config');
$dom->appendChild($root);
$element = $dom->createElement('name',$conf->account_manager->name);
$root->appendChild($element);
$element = $dom->createElement('master_url',$conf->account_manager->master_url);
$root->appendChild($element);
$element = $dom->createElement('min_passwd_length',$conf->account_manager->min_passwd_length);
$root->appendChild($element);
$element = $dom->createElement('account_manager');
$root->appendChild($element);
if ($conf->account_manager->usernames == '1')
    {
    $element = $dom->createElement('uses_username');
    $root->appendChild($element);
    }

// if project has a min client version, enforce it
$min_core_client_version = (int) $conf->account_manager->min_core_client_version;
if ($min_core_client_version){
    $x = $_SERVER['HTTP_USER_AGENT'];
    list($platform, $maj, $min)  = sscanf($x, "BOINC client (%s %d.%d)");
    $too_old = false;
    if ($maj !== null && $min !== null) {
        $v = $maj*100 + $min;
        if ($v < $min_core_client_version) {
            $too_old = true;
        }
    } else {
        $too_old = true;
    }
    if ($too_old) {
        $rmaj = $min_core_client_version/100;
        $rmin = $min_core_client_version%100;
        error_log("get_project_config.php: [-190] This project requires BOINC client version $rmaj.$rmin or later.");
        // There is not <error_msg> item in the documentation, so for now it is not reported.
        //$element = $dom->createElement('error_msg',"This project requires BOINC client version $rmaj.$rmin or later.");
        //$root->appendChild($element);
        $element = $dom->createElement('error_num',"-190");
        $root->appendChild($element);
    }
}
echo $dom->saveXML();
?>
