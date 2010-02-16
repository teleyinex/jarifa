<?php 
/* 
Copyright 2010 Daniel Lombraña González

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

class identica 
{
    public $conf;

    
    function __construct($language,$username=null,$password=null)
    {
        $this->language=$language;
        putenv("LANG=$this->language");
        setlocale(LC_ALL, $this->language);
        $domain = 'messages';
        bindtextdomain($domain, "lang");
        textdomain($domain);
        $this->load_config();
    }

    function load_config()
    {
          $this->conf = simplexml_load_file("conf/jarifa.conf");
          // Are we using a proxy?
          if ($this->conf->xpath("/conf/proxy"))
          {
            error_log("Using proxy configuration");
            $this->proxy = True;
            $this->proxy_name = $this->conf->proxy->name;
            $this->proxy_port = $this->conf->proxy->port;
            if ($this->conf->proxy->request_fulluri == 1) $this->proxy_request_fulluri = True;
            else $this->proxy_request_fulluri = False;
            $opts= array('http' => array('request_fulluri' => $this->proxy_request_fulluri,'proxy' =>'tcp://'.$this->proxy_name.':'.$this->proxy_port));
            $this->context = stream_context_create($opts);
          }
          else $this->context = null;
    }

    function update_status($message)
    {
        // Create the curl handler to the authentication and update service of Identi.ca
        $ch = curl_init("https://identi.ca/api/statuses/update.xml");

        if ($this->proxy)
        {
            curl_setopt($ch, CURLOPT_PROXY, $this->conf->proxy->name);
            curl_setopt($ch, CURLOPT_PROXYPORT, $this->conf->proxy->port);
        }
        // Set credentials
        curl_setopt($ch,CURLOPT_USERPWD,$this->conf->identica->username.":".$this->conf->identica->password);
        curl_setopt($ch,CURLOPT_POST,1);
        // Post the message
        curl_setopt($ch,CURLOPT_POSTFIELDS,"&source=".$this->conf->identica->source."&status=$message");
        curl_setopt($ch,CURLOPT_RETURNTRANSFER,1);
        $output = curl_exec($ch);
        curl_close($ch);
    }

}
?>
