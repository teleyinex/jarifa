-- phpMyAdmin SQL Dump
-- version 2.11.3deb1ubuntu1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Aug 06, 2008 at 02:22 PM
-- Server version: 5.0.51
-- PHP Version: 5.2.4-2ubuntu5.3

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

--
-- Database: `ogm`
--

-- --------------------------------------------------------

--
-- Table structure for table `host`
--

CREATE TABLE IF NOT EXISTS `host` (
  `id` int(11) NOT NULL auto_increment,
  `poolid` int(11) default NULL,
  `supplier` varchar(255) collate utf8_bin NOT NULL,
  `DBID` varchar(255) collate utf8_bin default NULL,
  `CPID` varchar(255) collate utf8_bin default NULL,
  `venue` varchar(254) collate utf8_bin default NULL,
  PRIMARY KEY  (`id`),
  KEY `poolid` (`poolid`),
  KEY `CPID` (`CPID`),
  KEY `DBID` (`DBID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=13 ;

-- --------------------------------------------------------

--
-- Table structure for table `pool`
--

CREATE TABLE IF NOT EXISTS `pool` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(255) collate utf8_bin NOT NULL,
  `supplier` varchar(255) collate utf8_bin NOT NULL,
  `start_hour` varchar(5) collate utf8_bin NOT NULL default '0.0',
  `end_hour` varchar(5) collate utf8_bin NOT NULL default '0.0',
  `suspend_while_on_batteries` tinyint(1) NOT NULL default '1',
  `suspend_if_user_active` tinyint(1) NOT NULL default '1',
  `idle_time_to_run` int(11) NOT NULL default '3',
  `suspend_if_no_recent_input` int(11) NOT NULL default '0',
  `leave_apps_in_memory` tinyint(1) NOT NULL default '0',
  `cpu_scheduling_period_minutes` int(11) NOT NULL default '60',
  `max_cpus` int(11) NOT NULL default '16',
  `max_ncpus_pct` int(11) NOT NULL default '100',
  `cpu_usage_limit` int(11) NOT NULL default '100',
  `disk_max_used_gb` float NOT NULL default '100',
  `disk_min_free_gb` float NOT NULL default '0.001',
  `disk_max_used_pct` int(11) NOT NULL default '50',
  `disk_interval` int(11) NOT NULL default '60',
  `vm_max_used_pct` int(11) NOT NULL default '75',
  `ram_max_used_busy_pct` int(11) NOT NULL default '50',
  `ram_max_used_idle_pct` int(11) NOT NULL default '90',
  `work_buf_min_days` int(11) NOT NULL default '0',
  `work_buf_additional_days` float NOT NULL default '0.25',
  `confirm_before_connecting` tinyint(1) NOT NULL default '0',
  `hangup_if_dialed` tinyint(1) NOT NULL default '0',
  `max_bytes_sec_down` float NOT NULL default '0',
  `max_bytes_sec_up` float NOT NULL default '0',
  `net_start_hour` varchar(5) collate utf8_bin NOT NULL default '0.0',
  `net_end_hour` varchar(5) collate utf8_bin NOT NULL default '0.0',
  `dont_verify_images` tinyint(1) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `department` (`supplier`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=24 ;

-- --------------------------------------------------------

--
-- Table structure for table `project`
--

CREATE TABLE IF NOT EXISTS `project` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(255) collate utf8_bin NOT NULL,
  `url` varchar(255) collate utf8_bin NOT NULL,
  `signature` varchar(1000) collate utf8_bin NOT NULL,
  `authenticator` varchar(255) collate utf8_bin NOT NULL,
  `share` tinyint(4) NOT NULL,
  `detach` tinyint(1) NOT NULL default '0',
  `update` tinyint(1) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=35 ;

-- --------------------------------------------------------

--
-- Table structure for table `user`
--

CREATE TABLE IF NOT EXISTS `user` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(255) collate utf8_bin NOT NULL,
  `password` varchar(32) collate utf8_bin NOT NULL,
  `role` varchar(255) collate utf8_bin NOT NULL,
  `supplier` varchar(255) collate utf8_bin NOT NULL,
  `supp_auth` varchar(255) collate utf8_bin NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=9 ;