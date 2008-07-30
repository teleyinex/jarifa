<?php 
class host 
{
    public $id;
    public $poolid;
    public $supplier;
    public $DBID;
    public $CPID;
}

class pool
{
    public $id;
    public $name;
    public $supplier;
    public $prefs;
    public $user_active;
    public $start_hour;
    public $end_hour;
    public $net_upload;
    public $net_download;
    public $hd_max;
    public $hd_left;
    public $ram_use_max;
    public $ram_idle_max;
}

class project
{
    public $id;
    public $url;
    public $signature;
    public $authenticator;
    public $share;
    public $detach;
    public $update;
}

class user
{
    public $id;
    public $password;
    public $role;
    public $supplier;
}
?>
