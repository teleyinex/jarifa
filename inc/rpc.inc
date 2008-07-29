<?php 
require_once("inc/db_conn.inc");
require_once("model/data.inc");

class rpc
{
    public $dom;
    public $xml;
    public $db;
    public $db_user='ogm';
    public $db_password='ogm';
    public $db_host='localhost';
    public $db_name='ogm';
    public $user;
    public $host;

    public function __construct()
    {
        $this->db = new DbConn();
        $this->db->init_conn($this->db_user,$this->db_password,$this->db_host,$this->db_name);
        $this->user = new user();
        $this->host = new host();
        $this->dom = new DOMDocument('1.0', 'iso-8859-1');
        $this->dom->formatOutput = true;
        $this->xml = $this->dom->createElement('acct_mgr_reply');
        $this->dom->appendChild($this->xml);
    }
    
    function auth($host)
    {
        $obj = $this->db->lookup_fields("user","user","*",'supp_auth="'.$host->password_hash.'"');
        if (!empty($obj)) 
        {
            $this->host->supplier = $obj->supplier;
            $this->host->CPID = $host->host_cpid;
            $this->host->DBID = $host->host_dbid;
            $this->host->id = $host->opaque->id;
            return (True);
        }
        else return(False);
    }

    function exist_host()
    {
        $obj = $this->db->lookup_fields("host","host","*",'id="'.$this->host->id.'"');
        if (!empty($obj))
        {
            return (True);
        } 
        else return(False);
    }

    function add_host()
    {
        $obj = $this->db->lookup_fields("pool","pool","*",'supplier="'.$this->host->supplier.'" and name="default"');
        $this->host->poolid = $obj->id;
        $query = '(poolid,supplier,CPID,venue) values ('.$this->host->poolid.',"'.$this->host->supplier.'","'.$this->host->CPID.'",null)';
        $obj = $this->db->insert("host",$query);
        if (!empty($obj)) return ($this->db->insert_id());
        else return(False);
    }

    function xmlSigningKey()
    {
        $element = $this->dom->createElement('name','AM Server Daniel');
        $this->xml->appendChild($element);
        $element = $this->dom->createElement('signing_key','
1024
b440f5493bf962f8f6a3f857589f7a0db28497ce0c8053c5047412b0347b0b1e
010dd31f8980b4385b2bcc8babf466c5f02eb3f81555c612630fad817518f4a5
5b4af13a1d3f5c9dd87b3f1c9a0d426f4dd771f61f8d0b4b95e24f50ab24344c
cf85d9c9f04d6274ba0270593f235f52881a602694b122a522b08f7aa2e79491
0000000000000000000000000000000000000000000000000000000000000000
0000000000000000000000000000000000000000000000000000000000000000
0000000000000000000000000000000000000000000000000000000000000000
0000000000000000000000000000000000000000000000000000000000010001
.
');
        $this->xml->appendChild($element);
    }

    function xmlOpaqueID($id)
    {
        $opaque= $this->dom->createElement('opaque');
        $this->xml->appendChild($opaque);
        $element = $this->dom->createElement('id',$id);
        $opaque->appendChild($element);
    }
}


?>
