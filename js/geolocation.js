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

function onSuccess(position)
{
    document.user.city.value = position.address.city;
    document.user.country.value = position.address.country;
    document.user.state.value = position.address.region;

}

if (navigator.geolocation) {
    position = navigator.geolocation.getCurrentPosition(onSuccess);
}
