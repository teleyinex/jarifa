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

require_once("inc/db_conn.inc");
require_once("model/data.inc");

/**
 * class to handle an account manager rpc from boinc client.
 *
 */
class rpc
{
    public $dom;
    public $xml;
    public $db;
    public $db_user;
    public $db_password;
    public $db_host;
    public $db_name;
    public $ac_name;
    public $ac_public_signature;
    public $ac_repeat_sec;
    public $user;
    public $host;

    /**
     * constructor. initializes some members
     *
     */
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
        $conf = simplexml_load_file("conf/jarifa.conf");
        $this->db_user = $conf->db->user;
        $this->db_password = $conf->db->password;
        $this->db_host = $conf->db->host;
        $this->db_name = $conf->db->name;
        $this->ac_name = $conf->account_manager->name;
        $this->ac_public_signature = $conf->account_manager->public_signature;
        $this->ac_repeat_sec = $conf->account_manager->repeat_sec;
        $this->boinc_user_email = $conf->account_manager->boinc_user_email;
    }

    
    function auth($host)
    {
        error_log('start rpc->auth');
        error_log('hosts password_hash: '.$host->password_hash);
        $obj = $this->db->lookup_fields("user","user","*",'supp_auth="'.$host->password_hash.'"');
        if (!empty($obj))
        {
            error_log('db_lookup successful');
            $this->host->supplier = $obj->supplier;
            $this->host->CPID = $host->host_cpid;
            $this->host->DBID = $host->host_dbid;
            $this->host->id = $host->opaque->id;
            $this->host->name = $host->name;
            return (True);
        }
        else
        {
            error_log('db_lookup failed');
            return(False);
        }
    }

    function exist_host()
    {
        $obj = $this->db->lookup_fields("host","host","*",'id="'.$this->host->id.'"');
        if (!empty($obj))
        {
            $query = 'CPID="'.$this->host->CPID.'"';
            $query = $query . ', host_cpid="'.md5($this->host->CPID.$this->boinc_user_email).'"';
            $obj = $this->db->update($obj,"host",$query);
            return (True);
        }
        else return(False);
    }

    function add_host()
    {
        $obj = $this->db->lookup_fields("pool","pool","*",'supplier="'.$this->host->supplier.'" and name="default"');
        $this->host->poolid = $obj->id;
        $query = '(username,poolid,supplier,CPID,host_cpid,venue) values
            ("'.$this->host->name.'",'.$this->host->poolid.',"'.$this->host->supplier.'","'.$this->host->CPID.'","'.md5($this->host->CPID.$this->boinc_user_email).'", null)';
        $obj = $this->db->insert("host",$query);
        $this->host->id = $this->db->insert_id();
        if (!empty($obj)) return ($this->db->insert_id());
        else return(False);
    }

    function xmlSigningKey()
    {
        $element = $this->dom->createElement('name',$this->ac_name);
        $this->xml->appendChild($element);
        $element = $this->dom->createElement('signing_key',$this->ac_public_signature);
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

    function xmlRepeat_sec()
    {
        $element = $this->dom->createElement("repeat_sec",$this->ac_repeat_sec);
        $this->xml->appendChild($element);
    }

    function xmlPreferences()
    {
        $this->host = $this->db->lookup_fields('host','host','*','id='.$this->host->id);
        $prefs = $this->db->lookup_fields('pool','pool','*','id='.$this->host->poolid);
        if (!empty($prefs))
        {
           $global_preferences = $this->dom->createElement('global_preferences');
           $this->xml->appendChild($global_preferences);
           $element = $this->dom->createElement('mod_time',time());
           $global_preferences->appendChild($element);
           if ($prefs->suspend_while_on_batteries=='0')
           {
            $element = $this->dom->createElement('run_on_batteries');
            $global_preferences->appendChild($element);
           }
           if ($prefs->suspend_if_user_active=='0')
           {
            $element = $this->dom->createElement('run_if_user_active');
            $global_preferences->appendChild($element);
           }
           $element = $this->dom->createElement('idle_time_to_run',$prefs->idle_time_to_run);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('suspend_if_no_recent_input',$prefs->suspend_if_no_recent_input);
           $global_preferences->appendChild($element);
           if ($prefs->leave_apps_in_memory=='1')
           {
            $element = $this->dom->createElement('leave_apps_in_memory');
            $global_preferences->appendChild($element);
           }
           $element = $this->dom->createElement('cpu_scheduling_period_minutes',$prefs->cpu_scheduling_period_minutes);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('max_cpus',$prefs->max_cpus);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('max_ncpus_pct',$prefs->max_ncpus_pct);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('cpu_usage_limit',$prefs->cpu_usage_limit);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('disk_max_used_gb',$prefs->disk_max_used_gb);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('disk_min_free_gb',$prefs->disk_min_free_gb);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('disk_max_used_pct',$prefs->disk_max_used_pct);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('disk_interval',$prefs->disk_interval);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('vm_max_used_pct',$prefs->vm_max_used_pct);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('ram_max_used_busy_pct',$prefs->ram_max_used_busy_pct);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('ram_max_used_idle_pct',$prefs->ram_max_used_idle_pct);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('work_buf_min_days',$prefs->work_buf_min_days);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('work_buf_additional_days',$prefs->work_buf_additional_days);
           $global_preferences->appendChild($element);
           if ($prefs->confirm_before_connecting=='1')
           {
            $element = $this->dom->createElement('confirm_before_connecting');
            $global_preferences->appendChild($element);
           }
           if ($prefs->hangup_if_dialed=='1')
           {
            $element = $this->dom->createElement('hangup_if_dialed');
            $global_preferences->appendChild($element);
           }
           $element = $this->dom->createElement('max_bytes_sec_up',$prefs->max_bytes_sec_up*1024);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('max_bytes_sec_down',$prefs->max_bytes_sec_down*1024);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('net_start_hour',$prefs->net_start_hour);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('net_end_hour',$prefs->net_end_hour);
           if ($prefs->dont_verify_images=='1')
           {
            $element = $this->dom->createElement('dont_verify_images');
            $global_preferences->appendChild($element);
           }
           $element = $this->dom->createElement('start_hour',$prefs->start_hour);
           $global_preferences->appendChild($element);
           $element = $this->dom->createElement('end_hour',$prefs->end_hour);
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
    
    function error($errormsg) {
        $element = $this->dom->createElement('error',$errormsg);        
        $this->xml->appendChild($element);
    }
    
}
?>
