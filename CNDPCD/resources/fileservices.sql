/*
SQLyog Ultimate v9.60 
MySQL - 5.0.22-community-nt : Database - policy
*********************************************************************
*/
/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`policy` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `fileservices`;

/*Table structure for table `activity` */

DROP TABLE IF EXISTS `activity`;

CREATE TABLE `activity` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(20) default NULL,
  `elementParent` varchar(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


/*Table structure for table `context` */

DROP TABLE IF EXISTS `context`;

CREATE TABLE `context` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(80) default NULL,
  `elementParent` varchar(80) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



/*Table structure for table `measure` */

DROP TABLE IF EXISTS `measure`;

CREATE TABLE `measure` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(20) default NULL,
  `elementParent` varchar(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



/*Table structure for table `policy` */

DROP TABLE IF EXISTS `policy`;

CREATE TABLE `policy` (
  `id` int(11) NOT NULL auto_increment,
  `type` int(11) default NULL,
  `context` varchar(20) default NULL,
  `role` varchar(20) default NULL,
  `view` varchar(20) default NULL,
  `activity` varchar(20) default NULL,
  `measure` varchar(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



/*Table structure for table `result_conflict` */

DROP TABLE IF EXISTS `result_conflict`;

CREATE TABLE `result_conflict` (
  `id` int(11) NOT NULL auto_increment,
  `policyAmount` int(11) default NULL,
  `conflictAmount` int(11) default NULL,
  `unrelation` int(11) default NULL,
  `syntaxConflict` int(11) default NULL,
  `inclusiveConflict` int(11) default NULL,
  `partialConflict` int(11) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


/*Table structure for table `role` */

DROP TABLE IF EXISTS `role`;

CREATE TABLE `role` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(20) default NULL,
  `elementParent` varchar(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



/*Table structure for table `semantic_activity` */

DROP TABLE IF EXISTS `semantic_activity`;

CREATE TABLE `semantic_activity` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(20) default NULL,
  `elementSemantic` varchar(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



/*Table structure for table `semantic_context` */

DROP TABLE IF EXISTS `semantic_context`;

CREATE TABLE `semantic_context` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(80) default NULL,
  `elementSemantic` varchar(80) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


/*Table structure for table `semantic_mapping` */

DROP TABLE IF EXISTS `semantic_mapping`;

CREATE TABLE `semantic_mapping` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(20) default NULL,
  `elementSemantic` varchar(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


/*Table structure for table `semantic_measure` */

DROP TABLE IF EXISTS `semantic_measure`;

CREATE TABLE `semantic_measure` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(20) default NULL,
  `elementSemantic` varchar(20) default NULL,
  `type` int(11) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


/*Table structure for table `semantic_role` */

DROP TABLE IF EXISTS `semantic_role`;

CREATE TABLE `semantic_role` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(20) default NULL,
  `elementSemantic` varchar(100) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



/*Table structure for table `semantic_view` */

DROP TABLE IF EXISTS `semantic_view`;

CREATE TABLE `semantic_view` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(80) default NULL,
  `elementSemantic` varchar(80) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



/*Table structure for table `view` */

DROP TABLE IF EXISTS `view`;

CREATE TABLE `view` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(80) default NULL,
  `elementParent` varchar(80) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
