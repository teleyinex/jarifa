<?php

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
