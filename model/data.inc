<?php 
/* 
Copyright 2008 Daniel Lombraña González, David Anderson, Franisco Fernández de Vega

This file is part of OGM.

OGM is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OGM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with OGM.  If not, see <http://www.gnu.org/licenses/>.
*/

class host 
{
    public $id;
    public $poolid;
    public $supplier;
    public $CPID;
    public $host_cpid;
    public $p_ncpus;
    public $p_vendor;
    public $p_model;
    public $p_fpops;
    public $p_iops;
    public $os_name;
    public $os_version;

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

class stats_user
{
    public $date;
    public $project;
    public $total_credit;
    public $expavg_credit;
    public $expavg_time;
}

class stats_host
{
    public $date;
    public $id;
    public $host_cpid;
    public $supplier;
    public $project;
    public $total_credit;
    public $expavg_credit;
    public $expavg_time;
}
?>
