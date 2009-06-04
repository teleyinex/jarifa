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

class map 
{
    public $key;
    public $users;

    function __construct($key=null)
    {
        $this->key=$key;
    }

    function initialize($LatLng,$zoom)
    {
        printf('
            function initialize() {
              if (GBrowserIsCompatible()) {
                    var map = new GMap2(document.getElementById("map_canvas"));
                        map.addControl(new GLargeMapControl());
                        map.setCenter(new GLatLng('.$LatLng[0].','.$LatLng[1].'), '.$zoom.');

var volunteer = new GIcon();
volunteer.image = "./img/volunteer.png"
volunteer.iconSize = new GSize(38, 38);
volunteer.iconAnchor = new GPoint(18, 34);
volunteer.infoWindowAnchor = new GPoint(18,0);

var supplier = new GIcon();
supplier.image = "./img/supplier.png"
supplier.iconSize = new GSize(38, 38);
supplier.iconAnchor = new GPoint(18, 34);
supplier.infoWindowAnchor = new GPoint(18,0);


                    ');
    // Function for adding the event of clicking the icons and pop-up a window with the name of the user
    printf('
        
        function createMarker(map,point,icon,user) {
      var marker = new GMarker(point,icon);
      map.addOverlay(marker);
      GEvent.addListener(marker, "click", function() {
        var myHtml = "<b>" + user + "<\/b><br\/>";
        map.openInfoWindowHtml(point, myHtml);
      });
      return marker;
    }

    
    ');
       $user_LatLng = $this->markers();

       foreach ($user_LatLng as $user => $coordinates)
       {
                printf('var point = new GLatLng('.$coordinates[0].'+Math.random()*0.005,'.$coordinates[1].'+Math.random()*0.005);');
                printf("\n");
                // We check if the role is volunteer (green arrow) or supplier (purple arrow)
                if ($user_LatLng[$user][2]=="volunteer")
                {
                      printf('createMarker(map,point,volunteer,"'.$user.'");');  
                      printf("\n");
                }
                else
                {
                      printf('createMarker(map,point,supplier,"'.$user.'");');  
                      printf("\n");
                }
                                
       }
       printf('             
                  }
                }
            ');
    }

      function coordinates($location)
      {
          // Web RPC to conver a location into longitude and latitude
          // Escape blanks with plus chars
          $location=str_replace(" ","+",$location);
          $request='q='.$location.'&output=csv&key='.$this->key;
          $LatLng = file_get_contents(trim('http://maps.google.com/maps/geo?').$request, false,$this->context);
          $LatLng = explode(",",$LatLng);
          // Latitude
          $aux[0] = $LatLng[2];
          // Longitude
          $aux[1] = $LatLng[3];
          return($aux);
      }


    function markers()
    {
        foreach($this->users as $user)
        {
            $user_LatLng[$user->name]=array($user->latitude,$user->longitude,$user->role);
        }
        return($user_LatLng);
    }


    function create($users,$country)
    {
        $this->users = $users;
        printf('<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key='.$this->key.'"
                type="text/javascript"></script>
                <script
src="http://gmaps-utility-library.googlecode.com/svn/trunk/markermanager/release/src/markermanager.js" type="text/javascript"></script>
                <script type="text/javascript">
                //<![CDATA[
              ');
        $LatLng = $this->coordinates($country);
        $this->initialize($LatLng,1);
    }

}
?>
