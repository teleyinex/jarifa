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
	 * Multiple horizontal bar chart demonstration.
	 *
	 */

	include "../libchart/classes/libchart.php";

	$chart = new VerticalBarChart();

	$serie1 = new XYDataSet();
	$serie1->addPoint(new Point("YT", 64));
	$serie1->addPoint(new Point("NT", 63));
	$serie1->addPoint(new Point("BC", 58));
	$serie1->addPoint(new Point("AB", 58));
	$serie1->addPoint(new Point("SK", 46));
	
	$serie2 = new XYDataSet();
	$serie2->addPoint(new Point("YT", 61));
	$serie2->addPoint(new Point("NT", 60));
	$serie2->addPoint(new Point("BC", 56));
	$serie2->addPoint(new Point("AB", 57));
	$serie2->addPoint(new Point("SK", 52));
	
	$dataSet = new XYSeriesDataSet();
	$dataSet->addSerie("1990", $serie1);
	$dataSet->addSerie("1995", $serie2);
	$chart->setDataSet($dataSet);
	$chart->getPlot()->setGraphCaptionRatio(0.65);

	$chart->setTitle("Average family income (k$)");
	$chart->render("generated/demo7.png");
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>Libchart line demonstration</title>
	<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-15" />
</head>
<body>
	<img alt="Line chart" src="generated/demo7.png" style="border: 1px solid gray;"/>
</body>
</html>
