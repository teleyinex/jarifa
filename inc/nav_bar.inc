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

class nav_bar 
{
    // Creates a navigation bar for data grids
    function create_num_bar($obj,$pos=0,$num=0,$init=0,$action)
    {
         if (count($obj)>$num)
        {
            $total = count($obj);
            $pags = (int)round($total/$num);
            $nav = "";
            if ($init!=0)
                $j=$init-5;
            else
                $j = 0;
            if ($init>5)
            {
                $nav = $nav . '<a href="index.php?action='.$action.'&amp;pos='.(($pos)-($num*5)).'&amp;num='.$num.'&amp;init='.($init-5).'">...</a> ';
            }

            for($i=0;$i<5;$i++)
            {
                $position = $j*$num;
                if ((($position/$num)) < $pags)
                {
                    $nav = $nav . '<a href="index.php?action='.$action.'&amp;pos='.$position.'&amp;num='.$num.'&amp;init='.$init.'">'.(($position/$num)+1).'</a> ';
                    $j+=1;
                }
            }
            if ($j<$pags)
                $nav = $nav . '<a href="index.php?action='.$action.'&amp;pos='.$j*$num.'&amp;num='.$num.'&amp;init='.($init+5).'">...</a> ';
        print('<div id="nav" align="center">'.$nav.'</div>');
        $obj=array_slice($obj,$pos,$num);
        return($obj);
        }
        else
             return($obj);
    }
}
?>
