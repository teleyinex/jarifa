<?php
$language='en_US.utf8';

putenv("LANG=$language");
setlocale(LC_ALL, $language);
$domain = 'messages';
bindtextdomain($domain, "locale"); 
textdomain($domain);

?>
