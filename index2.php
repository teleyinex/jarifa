<?php

session_start();

if (isset($_SESSION['role']))
{
    printf("Hola Usuario: ".$_SESSION['userid']. " con rol ".$_SESSION['role']);
    printf("<form method=\"post\" action=\"logout.php\">
        <input type=\"submit\"/>");
}
else
printf("<form method=\"post\" action=\"login.php\" name=\"login\">
    Login: <input name=\"login\"><br>
    Password:<input name=\"password\" type=\"password\">
    <input type=\"submit\"/>
    </form>
    ");

?>
