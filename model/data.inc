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
    public $suspend_while_on_batteries;
    public $suspend_if_user_active;
    public $idle_time_to_run;
    public $suspend_if_no_recent_input;
    public $leave_apps_in_memory;
    public $cpu_scheduling_period_minutes;
    public $max_cpus;
    public $max_ncpus_pct;
    public $cpu_usage_limit;
    public $disk_max_used_gb;
    public $disk_min_free_gb;
    public $disk_max_used_pct;
    public $disk_interval;
    public $vm_max_used_pct;
    public $ram_max_used_busy_pct;
    public $ram_max_used_idle_pct;
    public $work_buf_min_days;
    public $work_buf_additional_days;
    public $confirm_before_connecting;
    public $hangup_if_dialed;
    public $max_bytes_sec_down;
    public $max_bytes_sec_up;
    public $net_start_hour;
    public $net_end_hour;
    public $dont_verify_images;
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
