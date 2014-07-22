
SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

--
-- Database: `sbc`
--
CREATE DATABASE `sbc` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

USE `sbc`;

-- --------------------------------------------------------

--
-- User : `sbc` with password `sbcsbc`
--

CREATE USER sbc IDENTIFIED BY 'sbcsbc';

GRANT CREATE,INSERT,SELECT,UPDATE,DELETE ON sbc.* TO 'sbc'@'localhost' IDENTIFIED BY 'sbcsbc';

-- --------------------------------------------------------

--
-- Table structure for table `current_calls`
--

CREATE TABLE IF NOT EXISTS `current_calls` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_from` varchar(64) NOT NULL,
  `user_to` varchar(64) NOT NULL,
  `server_from` varchar(128) NOT NULL,
  `server_to` varchar(128) NOT NULL,
  `start_time` timestamp NULL DEFAULT NULL,
  `uuid` varchar(255) DEFAULT NULL,
  `CREATION_DATE` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=MEMORY  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `logs_calls`
--

CREATE TABLE IF NOT EXISTS `logs_calls` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_from` varchar(64) NOT NULL,
  `user_to` varchar(64) NOT NULL,
  `server_from` varchar(128) NOT NULL,
  `server_to` varchar(128) NOT NULL,
  `duration` int(11) NOT NULL DEFAULT '0',
  `start_time` timestamp NULL DEFAULT NULL,
  `end_time` timestamp NULL DEFAULT NULL,
  `CREATION_DATE` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `logs_sms`
--

CREATE TABLE IF NOT EXISTS `logs_sms` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_from` varchar(64) NOT NULL,
  `user_to` varchar(64) NOT NULL,
  `server_from` varchar(128) NOT NULL,
  `server_to` varchar(128) NOT NULL,
  `data` text,
  `CREATION_DATE` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `routes`
--

CREATE TABLE IF NOT EXISTS `routes` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `number` char(16) NOT NULL,
  `dest` varchar(64) NOT NULL,
  `CREATION_DATE` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

