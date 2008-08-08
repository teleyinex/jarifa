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
	
	/*
	 * Vertical bar chart demonstration
	 *
	 */

	include "../libchart/classes/libchart.php";

	$chart = new VerticalBarChart();

	$dataSet = new XYDataSet();
	$dataSet->addPoint(new Point("Jan 2005", 273));
	$dataSet->addPoint(new Point("Feb 2005", 421));
	$dataSet->addPoint(new Point("March 2005", 642));
	$dataSet->addPoint(new Point("April 2005", 800));
	$dataSet->addPoint(new Point("May 2005", 1200));
	$dataSet->addPoint(new Point("June 2005", 1500));
	$dataSet->addPoint(new Point("July 2005", 2600));
	$chart->setDataSet($dataSet);

	$chart->setTitle("Monthly usage for www.example.com");
	$chart->render("generated/demo1.png");
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>Libchart vertical bars demonstration</title>
	<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-15" />
</head>
<body>
	<img alt="Vertical bars chart" src="generated/demo1.png" style="border: 1px solid gray;"/>
</body>
</html>
