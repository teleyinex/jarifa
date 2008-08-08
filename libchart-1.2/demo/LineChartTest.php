<?php
	/* Libchart - PHP chart library
	 * Copyright (C) 2005-2007 Jean-Marc Trémeaux (jm.tremeaux at gmail.com)
	 * 
	 * This program is free software: you can redistribute it and/or modify
	 * it under the terms of the GNU General Public License as published by
	 * the Free Software Foundation, either version 3 of the License, or
	 * (at your option) any later version.
	 * 
	 * This program is distributed in the hope that it will be useful,
	 * but WITHOUT ANY WARRANTY; without even the implied warranty of
	 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 * GNU General Public License for more details.
	 *
	 * You should have received a copy of the GNU General Public License
	 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
	 * 
	 */
	
	/**
	 * Line chart demonstration
	 *
	 */

	include "../libchart/classes/libchart.php";

	$chart = new LineChart();

	$dataSet = new XYDataSet();
	$dataSet->addPoint(new Point("06-01", 273));
	$dataSet->addPoint(new Point("06-02", 421));
	$dataSet->addPoint(new Point("06-03", 642));
	$dataSet->addPoint(new Point("06-04", 799));
	$dataSet->addPoint(new Point("06-05", 1009));
	$dataSet->addPoint(new Point("06-06", 1406));
	$dataSet->addPoint(new Point("06-07", 1820));
	$dataSet->addPoint(new Point("06-08", 2511));
	$dataSet->addPoint(new Point("06-09", 2832));
	$dataSet->addPoint(new Point("06-10", 3550));
	$dataSet->addPoint(new Point("06-11", 4143));
	$dataSet->addPoint(new Point("06-12", 4715));
	$chart->setDataSet($dataSet);

	$chart->setTitle("Sales for 2006");
	$chart->render("generated/demo5.png");
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>Libchart line demonstration</title>
	<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-15" />
</head>
<body>
	<img alt="Line chart" src="generated/demo5.png" style="border: 1px solid gray;"/>
</body>
</html>
