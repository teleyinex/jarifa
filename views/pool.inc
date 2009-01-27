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

require_once("inc/html.inc");
require_once("model/data.inc");

class HTMLpool extends html
{
    public $role;
    public $db;
    public $user_supplier;
    public $table='pool';
    public $class='pool';
    public $language;

    function __construct($language,$role=null,$supplier=null,$db)
    {
        $this->language=$language;
        putenv("LANG=$this->language");
        setlocale(LC_ALL, $this->language);
        $domain = 'messages';
        bindtextdomain($domain, "lang");
        textdomain($domain);
        $this->role = $role;
        $this->supplier = $supplier;
        $this->db = $db;
        $this->language;
        $this->load_config();
    }

    function adapt_time($time,$action)
    {
        switch ($action)
        {
        case 'edit':
        {
            list($hour, $min) = split("\.",$time);
            $min = round(((int)($min)*60)/100);
            if (($hour) == '0') $hour = '00';
            if (($min) == '0') $min = '00';
            if ((strlen($hour)) == 1) $hour = '0'. $hour;
            if ((strlen($min)) == 1) $min = '0'. $min ;
            return($hour.':'.$min);
        }

        case 'insert':
        {
            list($hour, $min) = split(":",$time);
            $min =  ((int)($min)*100)/60;
            $min = round($min);
            return($hour.'.'.$min);
        }
       
        }
        
    }

    function page($view=null,$values=null)
    {

        switch ($view)
        {
            case 'edit':
            {
                $this->header(gettext("Pools"));
                $this->menu($this->role,gettext("Editing pool"));
                $obj = $this->get($values['id']);
                $suppliers = $this->suppliers();
                $this->html_edit($obj,$suppliers);
                break;
            }

            case 'insert':
            {
                $this->header(gettext("Pools"));
                $this->menu($this->role,gettext("Create a new pool"));
                $suppliers = $this->suppliers();
                $this->html_insert($suppliers); 
                break;
            }
            default:
            {
                $this->header(gettext("Welcome").' '.$_SESSION['userid']);
                $this->menu($this->role,gettext("Available Pools"));
                break;
            }

        }

    }

    function hosts_per_pool($id)
    {
        $hosts = $this->db->enum_fields('host','host','id','poolid='.$id,'order by id');
        return(count($hosts));
    }

    function table($obj)
    {
        printf('<table border=1 align=center cellpadding="4" class="form">');
        printf('<tr class="Hd"><td colspan=3></td><td align="center" colspan=2>'.gettext("Time").'<td align="center" colspan=2>'.gettext("Network").'
                <td align="center" colspan=2>'.gettext("Hard Disk Usage").'</td><td align="center"
                colspan=2>'.gettext("Memory Usage").'</td>');
        if ($this->role == 'root') printf('<td></td>');
        printf('</tr>
            <tr><td align="center">'.gettext("Name").'<td align="center">'.gettext("Hosts").'</td>
            </td><td align="center">'.gettext("Run Always").'</td>
            <td align="center">'.gettext("Start").'</td><td align="center">'.gettext("End").'</td>
            <td align="center">'.gettext("Upload").'</td><td align="center">'.gettext("Download").'</td>
            <td align="center">'.gettext("Max Space").'</td><td align="center">'.gettext("Left Space").'</td>
            <td align="center">'.gettext("Max Active").'</td><td align="center">'.gettext("Max Idle").'</td>');
        if ($this->role == 'root')
        {
            printf('<td align="center">'.gettext("Supplier").'</td>');
        }
        printf('</tr>');
        $row = count($obj);
        for ($i=0 ; $i<$row;$i++)
        {
            printf ("<tr>");
            printf ('<td align=center><a href=index.php?action=ed_pool&id='.$obj[$i]->id.'>'.$obj[$i]->name.'</a></td>
                ');
            printf ('<td align="center">'.$this->hosts_per_pool($obj[$i]->id).'</td>');
            if ($obj[$i]->suspend_if_user_active==0) printf('<td align=center>Yes</td>');
            else printf('<td align=center>No</td>');
            $obj[$i]->start_hour = $this->adapt_time($obj[$i]->start_hour,'edit');
            $obj[$i]->end_hour = $this->adapt_time($obj[$i]->end_hour,'edit');
            printf('
                <td align="center">'.$obj[$i]->start_hour.'</td>
                <td align="center">'.$obj[$i]->end_hour.'</td>
                <td align="center">'.$obj[$i]->max_bytes_sec_up.' KB/s</td>
                <td align="center">'.$obj[$i]->max_bytes_sec_down.' KB/s</td>
                <td align="center">'.$obj[$i]->disk_max_used_gb.' GB</td>
                <td align="center">'.$obj[$i]->disk_min_free_gb.' GB</td>
                <td align="center">'.$obj[$i]->ram_max_used_busy_pct.' %%</td>
                <td align="center">'.$obj[$i]->ram_max_used_idle_pct.' %%</td>'
                  );
            if ($this->role == 'root') 
                printf('<td align="center">'.$obj[$i]->supplier.'</td>');
            printf ("\n</tr>");
        }
        printf("</table>");
        printf('<div style="text-align: center;"><a href=index.php?action=vin_pool>'.gettext("New Pool").'</a></div>');
        $this->footer();
    }

    function html_edit($obj,$suppliers=null)
    {
    
        printf('<form method="post" action="index.php?action=up_pool&id='.$obj->id.'" name="pool">');
        printf('<table align="center" style="text-align: left;" border="1" cellpadding="2" cellspacing="2" class="form">');
        printf('<tr>
                    <td colspan="2" class="Hd">'.gettext("Pool Features").'</td>
                </tr>
               ');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("Pool Name").'</td>
                    <td><input name="name" value="'.$obj->name.'"></td>
                </tr>
              ');
        if ($this->role == 'root')
        {
            printf('
                   <tr>
                        <td style="text-align: right;">'.gettext("Supplier").'</td>
                        <td><select name="supplier">');
            for ($i=0;$i<count($suppliers);$i++)
            {
                if ($suppliers[$i]->supplier != "root")
                {
                if ($obj->supplier == $suppliers[$i]->supplier)
                    printf('<option value="'.$suppliers[$i]->supplier.'" selected="selected">'.$suppliers[$i]->supplier.'</option>');
                else
                    printf('<option value="'.$suppliers[$i]->supplier.'">'.$suppliers[$i]->supplier.'</option>');
                }
            }
            printf('</select></td></tr>');

        }

        printf('<tr>
                    <td colspan="2" class="Hd">'.gettext("Processor usage").'</td>
                </tr>
            ');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("Suspend work while computer is on battery power?<br>(matters
                    only for portable computers)").'</td>');
        if ($obj->suspend_while_on_batteries == 1)
            printf ('<td>'.gettext("Yes").'<input name="suspend_while_on_batteries" type="radio" value="1" checked>'
                          .gettext("No").'<input name="suspend_while_on_batteries"  type="radio" value="0"></td>
                </tr> 
              ');
        else
            printf ('<td>'.gettext("Yes").'<input name="suspend_while_on_batteries" type="radio" value="1">'
                          .gettext("No").'<input name="suspend_while_on_batteries"  type="radio" value="0" checked></td>
                    </tr>');
        printf('<tr>
            <td style="text-align: right;">'.gettext("Suspend work while computer is in use?").'</td>');
        if ($obj->suspend_if_user_active == 1)
            printf('<td>'.gettext("Yes").'<input name="suspend_if_user_active" type="radio" value="1" checked>'
                         .gettext("No").'<input  name="suspend_if_user_active" type="radio" value="0"></td>
                   </tr>');
        else
            printf('<td>'.gettext("Yes").'<input name="suspend_if_user_active" type="radio" value="1">'
                         .gettext("No").'<input  name="suspend_if_user_active" type="radio" value="0" checked></td>
                   </tr>');

        printf('<tr>
                    <td style="text-align: right;">'.gettext("'In use' means mouse/keyboard activity in last").'</td>
                    <td><input size="3" name="idle_time_to_run" value="'.$obj->idle_time_to_run.'"> '.gettext("minutes").'</td>
                </tr> 
               ');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("Suspend work if no mouse/keyboard activity in last<br>(Needed to enter low-power mode on some computers)
<br>Enforced by version 5.10.14+").'</td>
                    <td><input size="3" name="suspend_if_no_recent_input" value="'.$obj->suspend_if_no_recent_input.'"> '.gettext("minutes").'</td>
                </tr> 
            ');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("Do work only between the hours of <br>(no restriction if equal)").'</td>
                    <td><select name=start_hour>');
        for ($i=0;$i<24;$i++)
        {
            if ($obj->start_hour == $i) printf("<option value=$i selected> $i".":00");
            else printf("<option value=$i> $i".":00");
        }
        printf('</select> '.gettext("and").'<select name=end_hour>');

        for ($i=0;$i<24;$i++)
        {
            if ($obj->end_hour == $i) printf("<option value=$i selected> $i".":00");
            else printf("<option value=$i> $i".":00");
        }
        printf('</select>
                    </td>
                </tr>
            ');

        printf('<tr>
                    <td style="text-align: right;">'.gettext("Leave applications in memory while suspended?<br>(suspended
                    applications will consume swap space if 'yes')").'</td>');
        if ($obj->leave_apps_in_memory == 1)
            printf ('<td>'.gettext("Yes").' <input type=radio name=leave_apps_in_memory value="1" checked>'
                          .gettext("No").' <input  type=radio name=leave_apps_in_memory value="0"></td></tr>');
        else
            printf ('<td>'.gettext("Yes").' <input type=radio name=leave_apps_in_memory value="1">'
                          .gettext("No").' <input  type=radio name=leave_apps_in_memory value="0" checked></td></tr>');

        printf('<tr>
                    <td style="text-align: right;">'.gettext("Switch between applications every <br>(recommended: 60 minutes)").'</td>
                    <td><input size="3" name="cpu_scheduling_period_minutes"
                    value="'.$obj->cpu_scheduling_period_minutes.'"> '.gettext("minutes").'</td>
                </tr>
            ');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("On multiprocessors, use at most<br>Enforced by version 5.10 and earlier").'</td>
                    <td><input  size="3" name="max_cpus" value="'.$obj->max_cpus.'"> '.gettext("processors").'</td>
                </tr>
               ');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("On multiprocessors, use at most<br> Enforced by version 6.1+").'</td>
                    <td><input  size="3" name="max_ncpus_pct" value="'.$obj->max_ncpus_pct.'"> '.gettext("%% of the processors").'</td>
                </tr>
               ');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("Use at most <br>(Can be used to reduce CPU heat)<br>Enforced by version 5.6+").'</td>
                    <td><input  size="3" name="cpu_usage_limit" value="'.$obj->cpu_usage_limit.'"> '.gettext("percent of CPU time").'</td>
            
            ');

        printf('<tr>
                    <td colspan="2" class="Hd" >'.gettext("Disk and memory usage").'</td>
                </tr>
            ');

        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Use at most").'</td>
                    <td><input  size="3" name="disk_max_used_gb" value="'.$obj->disk_max_used_gb.'"> '.gettext("GBytes").'</td>
                </tr>
              ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Leave at least<br>(Values smaller than 0.001 are ignored)").'</td>
                    <td><input  size="3" name="disk_min_free_gb" value="'.$obj->disk_min_free_gb.'"> '.gettext("GBytes free").'</td>
                </tr>
               ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Use at most").'</td>
                    <td><input  size="3" name="disk_max_used_pct" value="'.$obj->disk_max_used_pct.'"> '.gettext("%% of total disk
                    space").'</td>
                </tr>
            ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Write to disk at most every").'</td>
                    <td><input  size="3" name="disk_interval" value="'.$obj->disk_interval.'"> '.gettext("seconds").'</td>
                </tr>
               ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Use at most").'</td>
                    <td><input  size="3" name="vm_max_used_pct" value="'.$obj->vm_max_used_pct.'"> '.gettext("%% of page file (swap
                    space)").'</td>
                </tr>
               ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Use at most<br>Enforced by version 5.8+").'</td>
                    <td><input  size="3" name="ram_max_used_busy_pct" value="'.$obj->ram_max_used_busy_pct.'"> '.gettext("%% of memory
                    when computer is in use").'</td>
                </tr>
            ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Use at most<br>Enforced by version 5.8+").'</td>
                    <td><input  size="3" name="ram_max_used_idle_pct" value="'.$obj->ram_max_used_idle_pct.'"> '.gettext("%% of memory
                    when computer is not in use").'</td>
                </tr>
              ');
        printf('
                <tr>
                    <td colspan="2" class="Hd">'.gettext("Network usage").'</td>
                </tr>
               ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Computer is connected to the Internet about every<br>(Leave blank or 0 if always
                    connected.<br>BOINC will try to maintain at least this much work.)").'</td>
                    <td><input  size="3" name="work_buf_min_days" value="'.$obj->work_buf_min_days.'"> '.gettext("days").'</td>
                </tr>
               ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Maintain enough work for an additional<br>Enforced by version 5.10+").'</td>
                    <td><input  size="3" name="work_buf_additional_days" value="'.$obj->work_buf_additional_days.'">
                    '.gettext("days").'</td>
                </tr>
               ');

        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Confirm before connecting to Internet?<br>(matters only if you have a modem, ISDN or VPN
                    connection)").'</td>');

        if ($obj->confirm_before_connecting == 1)
            printf('<td>'.gettext("Yes").'<input type=radio name="confirm_before_connecting" value="1" checked>'
                         .gettext("No").'<input  type=radio name="confirm_before_connecting" value="0"></td></tr>');
        else
            printf('<td>'.gettext("Yes").'<input type=radio name="confirm_before_connecting" value="1">'
                         .gettext("No").'<input  type=radio name="confirm_before_connecting" value="0" checked></td></tr>');

        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Disconnect when done?<br>(matters only if you have a modem, ISDN or VPN
                    connection)").'</td>');
        if ($obj->hangup_if_dialed == 1)
            printf('<td>'.gettext("Yes").'<input type=radio name="hangup_if_dialed" value="1">'
                         .gettext("No").'<input  type=radio name="hangup_if_dialed" value="0" checked></td></tr>');
        else
            printf('<td>'.gettext("Yes").'<input type=radio name="hangup_if_dialed" value="1">'
                         .gettext("No").'<input  type=radio name="hangup_if_dialed" value="0" checked></td></tr>');

        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Maximum download rate:").'</td>
                    <td><input  size="3" name="max_bytes_sec_down" value="'.$obj->max_bytes_sec_down.'"> '.gettext("Kbytes/sec").'</td>
                </tr>
               ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Maximum upload rate:").'</td>
                    <td><input  size="3" name="max_bytes_sec_up" value="'.$obj->max_bytes_sec_up.'"> '.gettext("Kbytes/sec").'</td>
                </tr>
               ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Use network only between the hours of<br>Enforced by version 4.46+<br>(no restriction if
                    equal)").'</td>
                    <td><select name=net_start_hour>');
        for ($i=0;$i<24;$i++)
        {
            if ($obj->net_start_hour == $i) printf("<option value=$i selected> $i".":00");
            else printf("<option value=$i> $i".":00");
        }
        printf('</select> '.gettext("and").'<select name=net_end_hour>');

        for ($i=0;$i<24;$i++)
        {
            if ($obj->net_end_hour == $i) printf("<option value=$i selected> $i".":00");
            else printf("<option value=$i> $i".":00");
        }
        printf('</select></td></tr>');
        printf('
               <tr>
                    <td style="text-align: right;">'.gettext("Skip image file verification?<br>Check this ONLY if your Internet provider modifies image
                    files (UMTS does this, for example).<br>Skipping verification reduces the security of BOINC.").'</td>');
        if ($obj->dont_verify_images == 1)
            printf('<td>'.gettext("Yes").'<input type=radio name="dont_verify_images" value="1" checked>'
                         .gettext("No").'<input type=radio  name="dont_verify_images" value="0"></td></tr>');
        else
            printf('<td>'.gettext("Yes").'<input type=radio name="dont_verify_images" value="1">'
                         .gettext("No").'<input type=radio  name="dont_verify_images" value="0" checked></td></tr>');
        printf('<tr><td style="text-align: right;">'.gettext("Delete").'</td>');
        printf('<td><input name="delete" type=checkbox></td>');
        printf('<tr><td colspan="2" align="center"><input type="submit"></td></tr></table></form>');
        $this->footer();
    }

    function html_insert($suppliers=null)
    {
        printf('<form method="post" action="index.php?action=in_pool">');
        printf('<table align="center" style="text-align: left;" border="1" cellpadding="2" cellspacing="2" class="form">');
        printf('<tr>
                    <td colspan="2" class="Hd">'.gettext("Pool Features").'</td>
                </tr>
               ');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("Pool Name").'</td>
                    <td><input name="name"></td>
                </tr>
              ');
        if ($this->role == 'root')
        {
            printf('
                   <tr>
                        <td style="text-align: right;">'.gettext("Supplier").'</td>
                        <td><select name="supplier">');
            for ($i=0;$i<count($suppliers);$i++)
            {
                if ($suppliers[$i]->supplier != "root")
                {
                    printf('<option value="'.$suppliers[$i]->supplier.'">'.$suppliers[$i]->supplier.'</option>');
                }
            }
            printf('</select></td></tr>');

        }

        printf('<tr>
                    <td colspan="2" class="Hd">'.gettext("Processor usage").'</td>
                </tr>
            ');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("Suspend work while computer is on battery power?<br>(matters
                    only for portable computers)").'</td>');
        printf ('<td>'.gettext("Yes").'<input name="suspend_while_on_batteries" type="radio" value="1" checked>'
                      .gettext("No").'<input name="suspend_while_on_batteries"  type="radio" value="0"></td>
                </tr> 
              ');
        printf('<tr>
            <td style="text-align: right;">'.gettext("Suspend work while computer is in use?").'</td>');
        printf('<td>'.gettext("Yes").'<input name="suspend_if_user_active" type="radio" value="1" checked>'
                     .gettext("No").'<input  name="suspend_if_user_active" type="radio" value="0"></td>
                   </tr>');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("'In use' means mouse/keyboard activity in last").'</td>
                    <td><input size="3" name="idle_time_to_run" value="3"> '.gettext("minutes").'</td>
                </tr> 
               ');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("Suspend work if no mouse/keyboard activity in last<br>(Needed to enter low-power mode on some computers)
<br>Enforced by version 5.10.14+").'</td>
                    <td><input size="3" name="suspend_if_no_recent_input" value="0"> '.gettext("minutes").'</td>
                </tr> 
            ');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("Do work only between the hours of <br>(no restriction if equal)").'</td>
                    <td><select name=start_hour>');
        for ($i=0;$i<24;$i++)
        {
            printf("<option value=$i> $i".":00");
        }
        printf('</select> '.gettext("and").'<select name=end_hour>');

        for ($i=0;$i<24;$i++)
        {
            printf("<option value=$i> $i".":00");
        }
        printf('</select>
                    </td>
                </tr>
            ');

        printf('<tr>
                    <td style="text-align: right;">'.gettext("Leave applications in memory while suspended?<br>(suspended
                    applications will consume swap space if 'yes')").'</td>');
            printf ('<td>'.gettext("Yes").' <input type=radio name=leave_apps_in_memory value="1">'
                          .gettext("No").' <input  type=radio name=leave_apps_in_memory value="0" checked></td></tr>');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("Switch between applications every <br>(recommended: 60 minutes)").'</td>
                    <td><input size="3" name="cpu_scheduling_period_minutes"
                    value="60"> '.gettext("minutes").'</td>
                </tr>
            ');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("On multiprocessors, use at most<br>Enforced by version 5.10 and earlier").'</td>
                    <td><input  size="3" name="max_cpus" value="16"> '.gettext("processors").'</td>
                </tr>
               ');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("On multiprocessors, use at most<br> Enforced by version 6.1+").'</td>
                    <td><input  size="3" name="max_ncpus_pct" value="100"> '.gettext("%% of the processors").'</td>
                </tr>
               ');
        printf('<tr>
                    <td style="text-align: right;">'.gettext("Use at most <br>(Can be used to reduce CPU heat)<br>Enforced by version 5.6+").'</td>
                    <td><input  size="3" name="cpu_usage_limit" value="100"> '.gettext("percent of CPU time").'</td>
            
            ');

        printf('<tr>
                    <td colspan="2" class="Hd" >'.gettext("Disk and memory usage").'</td>
                </tr>
            ');

        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Use at most").'</td>
                    <td><input  size="3" name="disk_max_used_gb" value="100"> '.gettext("GBytes").'</td>
                </tr>
              ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Leave at least<br>(Values smaller than 0.001 are ignored)").'</td>
                    <td><input  size="3" name="disk_min_free_gb" value="0.001"> '.gettext("GBytes free").'</td>
                </tr>
               ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Use at most").'</td>
                    <td><input  size="3" name="disk_max_used_pct" value="50"> '.gettext("%% of total disk
                    space").'</td>
                </tr>
            ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Write to disk at most every").'</td>
                    <td><input  size="3" name="disk_interval" value="60"> '.gettext("seconds").'</td>
                </tr>
               ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Use at most").'</td>
                    <td><input  size="3" name="vm_max_used_pct" value="75"> '.gettext("%% of page file (swap
                    space)").'</td>
                </tr>
               ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Use at most<br>Enforced by version 5.8+").'</td>
                    <td><input  size="3" name="ram_max_used_busy_pct" value="50"> '.gettext("%% of memory
                    when computer is in use").'</td>
                </tr>
            ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Use at most<br>Enforced by version 5.8+").'</td>
                    <td><input  size="3" name="ram_max_used_idle_pct" value="90"> '.gettext("%% of memory
                    when computer is not in use").'</td>
                </tr>
              ');
        printf('
                <tr>
                    <td colspan="2" class="Hd">'.gettext("Network usage").'</td>
                </tr>
               ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Computer is connected to the Internet about every<br>(Leave blank or 0 if always
                    connected.<br>BOINC will try to maintain at least this much work.)").'</td>
                    <td><input  size="3" name="work_buf_min_days" value="0"> '.gettext("days").'</td>
                </tr>
               ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Maintain enough work for an additional<br>Enforced by version 5.10+").'</td>
                    <td><input  size="3" name="work_buf_additional_days" value="0.25">
                    '.gettext("days").'</td>
                </tr>
               ');

        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Confirm before connecting to Internet?<br>(matters only if you have a modem, ISDN or VPN
                    connection)").'</td>');

        printf('<td>'.gettext("Yes").'<input type=radio name="confirm_before_connecting" value="1">'
                     .gettext("No").'<input  type=radio name="confirm_before_connecting" value="0" checked></td></tr>');

        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Disconnect when done?<br>(matters only if you have a modem, ISDN or VPN
                    connection)").'</td>');
        printf('<td>'.gettext("Yes").'<input type=radio name="hangup_if_dialed" value="1">'
                     .gettext("No").'<input  type=radio name="hangup_if_dialed" value="0" checked></td></tr>');

        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Maximum download rate:").'</td>
                    <td><input  size="3" name="max_bytes_sec_down" value="0"> '.gettext("Kbytes/sec").'</td>
                </tr>
               ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Maximum upload rate:").'</td>
                    <td><input  size="3" name="max_bytes_sec_up" value="0"> '.gettext("Kbytes/sec").'</td>
                </tr>
               ');
        printf('
                <tr>
                    <td style="text-align: right;">'.gettext("Use network only between the hours of<br>Enforced by version 4.46+<br>(no restriction if
                    equal)").'</td>
                    <td><select name=net_start_hour>');
        for ($i=0;$i<24;$i++)
        {
            printf("<option value=$i> $i".":00");
        }
        printf('</select> '.gettext("and").'<select name=net_end_hour>');

        for ($i=0;$i<24;$i++)
        {
            printf("<option value=$i> $i".":00");
        }
        printf('</select></td></tr>');
        printf('
               <tr>
                    <td style="text-align: right;">'.gettext("Skip image file verification?<br>Check this ONLY if your Internet provider modifies image
                    files (UMTS does this, for example).<br>Skipping verification reduces the security of BOINC.").'</td>');
        printf('<td>'.gettext("Yes").'<input type=radio name="dont_verify_images" value="1">'
                     .gettext("No").'<input type=radio  name="dont_verify_images" value="0" checked></td></tr>');
        printf('<tr><td colspan="2" align="center"><input type="submit"></td></tr></table></form>');
        $this->footer();

    }

    function suppliers()
    {
        $obj = $this->db->enum_fields('user','user','supplier','1','group by supplier');
        return($obj);
    }

    function lists()
      {
          if ($this->role == 'root')
            $obj = $this->db->enum_fields($this->table,$this->class,'*','1', 'order by id');
          else
            $obj = $this->db->enum_fields($this->table,$this->class,'*','supplier="'.$this->supplier.'"', 'order by id');
          return($obj);
      }

      function get($id)
      {
          return ($this->db->lookup_fields($this->table,$this->class,'*','id="'.$id.'"'));
      }

      function update($id,$values)
      {
        $obj = $this->db->lookup_id($id,$this->table,$this->class);

        $query = 'name="'.$values['name'].'"';
        if ($this->role == 'root') $query = $query . ', supplier="'.$values['supplier'].'"';
        $query = $query . ', start_hour='.$values['start_hour'];
        $query = $query . ', end_hour='.$values['end_hour'];
        $query = $query . ', suspend_while_on_batteries='.$values['suspend_while_on_batteries'];
        $query = $query . ', suspend_if_user_active='.$values['suspend_if_user_active'];
        $query = $query . ', idle_time_to_run='.$values['idle_time_to_run'];
        $query = $query . ', suspend_if_no_recent_input='.$values['suspend_if_no_recent_input'];
        $query = $query . ', leave_apps_in_memory='.$values['leave_apps_in_memory'];
        $query = $query . ', cpu_scheduling_period_minutes='.$values['cpu_scheduling_period_minutes'];
        $query = $query . ', max_cpus='.$values['max_cpus']; 
        $query = $query . ', max_ncpus_pct='.$values['max_ncpus_pct'];
        $query = $query . ', cpu_usage_limit='.$values['cpu_usage_limit'];
        $query = $query . ', disk_max_used_gb='.$values['disk_max_used_gb'];
        $query = $query . ', disk_min_free_gb='.$values['disk_min_free_gb'];
        $query = $query . ', disk_max_used_pct='.$values['disk_max_used_pct'];
        $query = $query . ', disk_interval='.$values['disk_interval'];
        $query = $query . ', vm_max_used_pct='.$values['vm_max_used_pct'];
        $query = $query . ', ram_max_used_busy_pct='.$values['ram_max_used_busy_pct'];
        $query = $query . ', ram_max_used_idle_pct='.$values['ram_max_used_idle_pct'];
        $query = $query . ', work_buf_min_days='.$values['work_buf_min_days'];
        $query = $query . ', work_buf_additional_days='.$values['work_buf_additional_days'];
        $query = $query . ', confirm_before_connecting='.$values['confirm_before_connecting'];
        $query = $query . ', hangup_if_dialed='.$values['hangup_if_dialed'];
        $query = $query . ', max_bytes_sec_down='.$values['max_bytes_sec_down'];
        $query = $query . ', max_bytes_sec_up='.$values['max_bytes_sec_up'];
        $query = $query . ', net_start_hour='.$values['net_start_hour'];
        $query = $query . ', net_end_hour='.$values['net_end_hour'];
        $query = $query . ', dont_verify_images='.$values['dont_verify_images'];
        return($this->db->update($obj,$this->table,$query));
      }


      function delete($id)
      {
        $obj = $this->db->lookup_id($id,$this->table,$this->class);
        return($this->db->delete($obj,$this->table));
      }

      function insert($values)
      {
          if ($this->role != 'root') $values['supplier']=$this->supplier;
          $query = '(name,
                     supplier,
                     start_hour,
                     end_hour,
                     suspend_while_on_batteries,
                     suspend_if_user_active,
                     idle_time_to_run,
                     suspend_if_no_recent_input,
                     leave_apps_in_memory,
                     cpu_scheduling_period_minutes,
                     max_cpus,
                     max_ncpus_pct,
                     cpu_usage_limit,
                     disk_max_used_gb,
                     disk_min_free_gb,
                     disk_max_used_pct,
                     disk_interval,
                     vm_max_used_pct,
                     ram_max_used_busy_pct,
                     ram_max_used_idle_pct,
                     work_buf_min_days,
                     work_buf_additional_days,
                     confirm_before_connecting,
                     hangup_if_dialed,
                     max_bytes_sec_down,
                     max_bytes_sec_up,
                     net_start_hour,
                     net_end_hour,
                     dont_verify_images) values ';
          $query = $query .'
                  ("'.$values['name'].'"'.
                   ',"'.$values["supplier"].
                   '",'.$values["start_hour"].
                   ','.$values["end_hour"].
                   ','.$values["suspend_while_on_batteries"].
                   ','.$values["suspend_if_user_active"].
                   ','.$values["idle_time_to_run"].
                   ','.$values["suspend_if_no_recent_input"].
                   ','.$values["leave_apps_in_memory"].
                   ','.$values["cpu_scheduling_period_minutes"].
                   ','.$values["max_cpus"].
                   ','.$values["max_ncpus_pct"].
                   ','.$values["cpu_usage_limit"].
                   ','.$values["disk_max_used_gb"].
                   ','.$values["disk_min_free_gb"].
                   ','.$values["disk_max_used_pct"].
                   ','.$values["disk_interval"].
                   ','.$values["vm_max_used_pct"].
                   ','.$values["ram_max_used_busy_pct"].
                   ','.$values["ram_max_used_idle_pct"].
                   ','.$values["work_buf_min_days"].
                   ','.$values["work_buf_additional_days"].
                   ','.$values["confirm_before_connecting"].
                   ','.$values["hangup_if_dialed"].
                   ','.$values["max_bytes_sec_down"].
                   ','.$values["max_bytes_sec_up"].
                   ','.$values["net_start_hour"].
                   ','.$values["net_end_hour"].
                   ','.$values["dont_verify_images"].'
                  )';
            return($this->db->insert($this->table, $query));
      }

}
?>
