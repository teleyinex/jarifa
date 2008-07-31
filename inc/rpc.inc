<?php 
require_once("inc/db_conn.inc");
require_once("model/data.inc");

class rpc
{
    public $dom;
    public $xml;
    public $db;
    public $db_user;
    public $db_password;
    public $db_host;
    public $db_name;
    public $user;
    public $host;

    public function __construct()
    {
        $this->db = new DbConn();
        $this->load_config();
        $this->db->init_conn($this->db_user,$this->db_password,$this->db_host,$this->db_name);
        $this->user = new user();
        $this->host = new host();
        $this->dom = new DOMDocument('1.0', 'utf-8');
        $this->dom->formatOutput = true;
        $this->xml = $this->dom->createElement('acct_mgr_reply');
        $this->dom->appendChild($this->xml);
    }

    function load_config()
    {
        $conf = simplexml_load_file("conf/ogm.conf");
        $this->db_user = $conf->user;
        $this->db_password = $conf->password;
        $this->db_host = $conf->host;
        $this->db_name = $conf->name;
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
        $this->host->id = $this->db->insert_id();
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

    function xmlOpaqueID()
    {
        $opaque= $this->dom->createElement('opaque');
        $this->xml->appendChild($opaque);
        $element = $this->dom->createElement('id',$this->host->id);
        $opaque->appendChild($element);
    }

    function projects()
    {
        $projects = $this->db->enum_fields("project","project","*","1","order by id");
        if (!empty($projects)) return ($projects);
        else return (False);
    }

    function xmlPreferences()
    {
        $this->host = $this->db->lookup_fields('host','host','*','id='.$this->host->id);
        $prefs = $this->db->lookup_fields('pool','pool','*','id='.$this->host->poolid);
        if (!empty($prefs))
        {
           $global_preferences = $this->dom->createElement('global_preferences');
           $this->xml->appendChild($global_preferences);
           $element = $this->dom->createElement('run_if_user_active',$prefs->user_active);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('start_hour',$prefs->start_hour);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('end_hour',$prefs->end_hour);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('max_bytes_sec_up',$prefs->net_upload*1024);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('max_bytes_sec_down',$prefs->net_download*1024);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('disk_max_used_gb',$prefs->hd_max);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('disk_min_free_gb',$prefs->hd_left);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('ram_max_used_busy_pct',$prefs->ram_use_max);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('ram_max_used_idle_pct',$prefs->ram_idle_max);
           $global_preferences->appendChild($element);
        }
        else
            return(False);

    }

    function xmlProject($project)
    {
        $account = $this->dom->createElement('account');
        $this->xml->appendChild($account);
        $element = $this->dom->createElement('url',$project->url);
        $account->appendChild($element);
        $element = $this->dom->createElement('url_signature',"$project->signature");
        $account->appendChild($element);
        $element = $this->dom->createElement('authenticator',$project->authenticator);
        $account->appendChild($element);
        $element = $this->dom->createElement('resource_share',$project->share);
        $account->appendChild($element);
        $element = $this->dom->createElement('detach',$project->detach);
        $account->appendChild($element);
        $element = $this->dom->createElement('update',$project->update);
        $account->appendChild($element);
    }
}
?>
