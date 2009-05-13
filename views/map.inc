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
require_once("inc/map.inc");

class HTMLmap extends html
{
    public $role;
    public $db;
    public $table='map';
    public $class='map';
    public $language;
    public $map;
    public $key;
    public $country;

    function __construct($language,$role=null,$db)
    {
        $this->language=$language;
        putenv("LANG=$this->language");
        setlocale(LC_ALL, $this->language);
        $domain = 'messages';
        bindtextdomain($domain, "lang");
        textdomain($domain);
        $this->role = "volunteer";
        $this->db = $db;
        $this->db->do_query("SET NAMES 'utf8'");
        $this->load_config();
        $this->map = new map($this->key);
    }

    function load_config()
    {
        $conf = simplexml_load_file("conf/jarifa.conf");
        $this->logo = $conf->account_manager->logo;
        $this->db_user = $conf->db->user;
        $this->db_password = $conf->db->password;
        $this->db_host = $conf->db->host;
        $this->db_name = $conf->db->name;
        $this->key = $conf->gmap->key;
        $this->country = $conf->gmap->country;
    }


    function page($view=null,$values=null)
    {
        switch ($view)
        {

            default:
            {
                $this->header(gettext("Volunteers on the map"));
                $this->html_map();
                break;
            }

        }

    }

    function html_map()
    {
        $users = $this->db->enum_fields('user','user','*','latitude!="" and longitude!="" and latitude!="0" and longitude!="0"', 'order by id');
        $this->map->create($users,$this->country);
        printf('//]]>');
        printf('</script>');
        printf('
                </head>
                <div id="legend" align="center">');
        printf('<table>
            <tr>
            <td>');
        printf(gettext("Volunteers"));
        printf('</td>
            <td><img src=./img/volunteer.png></td>');
        printf('<td></td><td>');
        printf(gettext("Suppliers"));
        printf('</td>
            <td><img src=./img/supplier.png></td>
            </tr>
            </table>');
        printf('</div>
                    
                    <div id="map_canvas" align="center" style="width: 600; height: 380px"></div>
                 </body>
               ');
    }

}
?>
