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
require_once("inc/poll.inc");

class HTMLpoll extends poll 
{
    public $role;
    public $db;
    public $table='user';
    public $class='user';
    public $language;

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
    }

    function page($view=null,$values=null)
    {
        switch ($view)
        {
            case 'results':
            {
                $this->html_result();
                break;
            }

            default:
            {
                $this->header(gettext("Vote for the most interesting project"));
                $this->html_poll();
                break;
            }

        }

    }

    function html_poll($userid)
    {
        // If the user has not voted, show the poll
        if ($this->check_vote($userid)=='0')
        {

            $projects = $this->get_projects();
            printf(gettext('Vote for the most interesting project:'));
            printf('<form method="post" action="index.php?action=vote" name="poll">');
            printf('<select name="vote">');
            foreach ($projects as $project)
            {
                printf('<option value="'.$project->name.'">'.$project->name.'</option>');
            }
            printf('</select>');
            printf('<input name="submit" type="submit" value="'.gettext("Vote").'"');
            printf('</form>');
        }
        else // show the results
        {
            $this->html_result();
        }
    }

    function html_result()
    {
        $this->draw_results();
        printf('<div align="center">');
        printf("<h3>");
        printf(gettext("Most voted projects"));
        printf("</h3><br>");
        printf('<img style="border:0;"  src="img/stats/votes.png" alt="votes">');
    }
}
?>
