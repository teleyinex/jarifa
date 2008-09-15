<?php
/* 
Copyright 2008 Daniel Lombraña González, David P. Anderson, Franisco Fernández de Vega

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
along with OGM.  If not, see <http://www.gnu.org/licenses/>.*/

$conf = simplexml_load_file("conf/ogm.conf");

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
    $element = $dom->createElement('user_username');
    $root->appendChild($element);
    }
echo $dom->saveXML();
?>
