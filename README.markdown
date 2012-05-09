Jarifa
======
Jarifa (a.k.a OGM) is a system for grid computing on organizational resources, using BOINC.

Jarifa is designed for situations where some entities that own computers (Suppliers) have decided to let another entities (the Allocator) decide how their computer time is to be divided among a set of BOINC projects. With Jarifa, Suppliers are able to control the usage of their computers (for example, the hours during which it does BOINC computation). However, they have no control over which BOINC projects their computers contribute to; the Allocator makes that decision.

The BOINC projects to which computing power is given need not be related to either Suppliers or Allocator; they might be public projects like Climateprediction.net or Rosetta@home.

Installation 
------------

The installation process of Jarifa is the following:

 * Download the source code.
 * Unzip it in /var/www or the document root of your Apache configuration file.
 * Set up the data base for Jarifa using the file located in /var/www/jarifa/conf/jarifa.sql.
 * Copy the file jarifa.conf.template to jarifa.conf and change the DB
   credentials if needed.
 * Log in using the following account: 
   `user name: root`
   `passowrd: root`
   .
 * Give Apache user read, write and access permissions to the Jarifa folder.
 * Access Jarifa with your web browser in: http://SERVER/jarifa 

Check the [wiki](https://github.com/teleyinex/jarifa/wiki) for further details.

See the 'INSTALL' file for more detailed directions.

Acknowledgments
---------------
This application was supported by:

 - University of Extremadura, CETA-Ciemat chair (SPAIN) and
 - University of California (USA).

