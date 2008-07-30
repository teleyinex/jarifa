<?php
$dom = new DOMDocument('1.0', 'iso-8859-1');
$dom->formatOutput = true;

$root = $dom->createElement('project_config');
$dom->appendChild($root);
$element = $dom->createElement('name','AM Server Daniel');
$root->appendChild($element);
$element = $dom->createElement('master_url','http://127.0.0.1/teleyinex/trunk</master_url>');
$root->appendChild($element);
$element = $dom->createElement('min_passwd_length','1');
$root->appendChild($element);
$element = $dom->createElement('account_manager');
$root->appendChild($element);
echo $dom->saveXML();
//<project_config>
//    <name>AM Server DANIEL</name>
//    <master_url>http://127.0.0.1/teleyinex/trunk</master_url>
//    <min_passwd_length>1</min_passwd_length> 
//    <account_manager/> 
//    <user_username/>
//</project_config>");
?>
