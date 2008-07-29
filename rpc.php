<?php 
require_once("inc/rpc.inc");

$rpc = new rpc;

$host = simplexml_load_file("php://input");

if ($rpc->auth($host))
{
    if (!$rpc->exist_host())
    {
        $id = $rpc->add_host();
        if ($id != False)
        {
            $rpc->xmlSigningKey();
            $rpc->xmlOpaqueID($id);
            echo $rpc->dom->saveXML();

        }
        else
            printf ("Error, no se ha añadido el host");
    }
    else
    {
            $rpc->xmlSigningKey();
            $rpc->xmlOpaqueID($rpc->host->id);
            echo $rpc->dom->saveXML();
    }
}
?>
