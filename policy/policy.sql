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

USE `policy`;

/*Table structure for table `activity` */

DROP TABLE IF EXISTS `activity`;

CREATE TABLE `activity` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(20) default NULL,
  `elementParent` varchar(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `activity` */

insert  into `activity`(`id`,`elementName`,`elementParent`) values (1,'ActivitySet','ProtocolService'),(2,'ActivitySet','FileService'),(3,'ProtocolService','HttpService'),(4,'ProtocolService','FtpService'),(5,'HttpService','Download'),(6,'FtpService','Upload'),(7,'FtpService','Download'),(8,'FileService','RWService'),(9,'FileService','RWEService'),(10,'FileService','RService'),(11,'RWService','Read'),(12,'RWService','Write'),(13,'RWEService','Write'),(14,'RWEService','Read'),(15,'RWEService','Execute'),(16,'RService','Read');

/*Table structure for table `context` */

DROP TABLE IF EXISTS `context`;

CREATE TABLE `context` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(80) default NULL,
  `elementParent` varchar(80) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `context` */

insert  into `context`(`id`,`elementName`,`elementParent`) values (1,'Default','ConScopeA'),(2,'Default','ConScopeB'),(3,'ConScopeA','ConScopeC'),(4,'ConScopeA','ConScopeD'),(5,'ConScopeB','ConScopeE'),(6,'ConScopeB','ConScopeF'),(7,'ConScopeC','ConScopeG'),(8,'ConScopeD','ConScopeG');

/*Table structure for table `measure` */

DROP TABLE IF EXISTS `measure`;

CREATE TABLE `measure` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(20) default NULL,
  `elementParent` varchar(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `measure` */

insert  into `measure`(`id`,`elementName`,`elementParent`) values (1,'permit','deny'),(2,'deny','permit'),(3,'permit','stop_service');

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

/*Data for the table `policy` */

insert  into `policy`(`id`,`type`,`context`,`role`,`view`,`activity`,`measure`) values (1,2,'Default','Members','ViewResource','ActivitySet','stop_service'),(2,0,'Default','Members','ViewResource','ActivitySet','permit'),(3,0,'ConScopeA','Manager','ServerSet1','ProtocolService','permit'),(4,0,'ConScopeC','SoftWare_Engineer','ServerSet1','HttpService','deny'),(5,0,'ConScopeD','QA_Engineer','ServerSet2','FtpService','permit'),(6,0,'ConScopeE','QA_Engineer','ToolResource','FileService','permit'),(7,0,'ConScopeC','SoftWare_Engineer','ServerSet1','HttpService','permit'),(8,0,'ConScopeA','Manager','ServerSet1','ProtocolService','permit'),(9,0,'ConScopeE','QA_Engineer','ToolResource','FileService','deny'),(10,0,'ConScopeF','UserB','S2','RWService','deny'),(11,0,'ConScopeF','UserC','S3','RService','permit'),(12,0,'ConScopeG','UserD','ToolResource','RWEService','deny'),(13,0,'ConScopeB','SoftWare_Engineer','ServerSet1','ProtocolService','permit'),(14,0,'ConScopeD','UserD','ViewResource','RWService','deny'),(15,0,'ConScopeB','Operation_Engineer','H1','Write','permit'),(16,0,'ConScopeD','UserA','S2','RService','permit'),(17,0,'ConScopeB','Projecter','H1','Write','deny'),(18,0,'ConScopeG','QA_Engineer','ToolResource','Download','deny'),(19,0,'ConScopeC','Projecter','P','FtpService','permit'),(20,0,'ConScopeF','Members','F','ActivitySet','deny'),(21,2,'Default','Members','ViewResource','ActivitySet','stop_service'),(22,2,'ConScopeB','Manager','ToolResource','HttpService','stop_service');

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

/*Data for the table `result_conflict` */

insert  into `result_conflict`(`id`,`policyAmount`,`conflictAmount`,`unrelation`,`syntaxConflict`,`inclusiveConflict`,`partialConflict`) values (6,5,5,5,1,3,1),(7,6,6,9,1,4,1),(8,7,8,13,1,5,2),(9,8,10,18,2,6,2),(10,9,12,24,3,7,2),(11,12,15,51,3,10,2),(12,20,27,163,3,19,5),(13,10,13,32,3,8,2),(14,11,14,41,3,9,2),(15,13,16,62,3,11,2),(16,14,17,74,3,12,2),(17,15,19,86,3,13,3),(18,16,20,100,3,14,3),(19,17,23,113,3,16,4),(20,18,24,129,3,17,4),(21,19,26,145,3,18,5),(22,21,27,183,3,19,5),(23,22,27,204,3,20,4);

/*Table structure for table `role` */

DROP TABLE IF EXISTS `role`;

CREATE TABLE `role` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(20) default NULL,
  `elementParent` varchar(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `role` */

insert  into `role`(`id`,`elementName`,`elementParent`) values (1,'Members','Manager'),(2,'Members','Projecter'),(3,'Manager','UserA'),(4,'Projecter','SoftWare_Engineer'),(5,'Projecter','QA_Engineer'),(6,'Projecter','Operation_Engineer'),(7,'SoftWare_Engineer','UserB'),(8,'SoftWare_Engineer','UserC'),(9,'QA_Engineer','UserB'),(10,'QA_Engineer','UserD'),(11,'Operation_Engineer','UserD');

/*Table structure for table `semantic_activity` */

DROP TABLE IF EXISTS `semantic_activity`;

CREATE TABLE `semantic_activity` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(20) default NULL,
  `elementSemantic` varchar(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `semantic_activity` */

insert  into `semantic_activity`(`id`,`elementName`,`elementSemantic`) values (1,'ActivitySet','SERVICE_ACCESS'),(2,'ProtocolService','web'),(3,'FileService','file'),(4,'HttpService','http'),(5,'FtpService',NULL),(6,'RWService',NULL),(7,'RWEService',NULL),(8,'RService',NULL),(9,'Download',NULL),(10,'Upload',NULL),(11,'Read',NULL),(12,'Write',NULL),(13,'Execute',NULL);

/*Table structure for table `semantic_context` */

DROP TABLE IF EXISTS `semantic_context`;

CREATE TABLE `semantic_context` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(80) default NULL,
  `elementSemantic` varchar(80) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `semantic_context` */

insert  into `semantic_context`(`id`,`elementName`,`elementSemantic`) values (1,'Default',''),(2,'ConScopeA',NULL),(3,'ConScopeB',NULL),(4,'ConScopeC',NULL),(5,'ConScopeD',NULL),(6,'ConScopeE',NULL),(7,'ConScopeF',NULL),(8,'ConScopeG',NULL);

/*Table structure for table `semantic_mapping` */

DROP TABLE IF EXISTS `semantic_mapping`;

CREATE TABLE `semantic_mapping` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(20) default NULL,
  `elementSemantic` varchar(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `semantic_mapping` */

/*Table structure for table `semantic_measure` */

DROP TABLE IF EXISTS `semantic_measure`;

CREATE TABLE `semantic_measure` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(20) default NULL,
  `elementSemantic` varchar(20) default NULL,
  `type` int(11) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `semantic_measure` */

insert  into `semantic_measure`(`id`,`elementName`,`elementSemantic`,`type`) values (1,'permit','',0),(2,'deny','',0),(3,'detect_ICMPFlood',NULL,1),(4,'detect_SYNFlood',NULL,1),(5,'detect_IPSpoof',NULL,1),(6,'detect_IPSpoof',NULL,1),(7,'stop_service',NULL,2);

/*Table structure for table `semantic_role` */

DROP TABLE IF EXISTS `semantic_role`;

CREATE TABLE `semantic_role` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(20) default NULL,
  `elementSemantic` varchar(100) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `semantic_role` */

insert  into `semantic_role`(`id`,`elementName`,`elementSemantic`) values (1,'Members','School'),(2,'Manager','Lab'),(3,'Projecter','666'),(4,'UserA','666'),(6,'SoftWare_Engineer','exnode'),(7,'QA_Engineer','externalnet'),(8,'Operation_Engineer','klbnt'),(9,'UserB','666'),(10,'UserC','666'),(11,'UserD','666');

/*Table structure for table `semantic_view` */

DROP TABLE IF EXISTS `semantic_view`;

CREATE TABLE `semantic_view` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(80) default NULL,
  `elementSemantic` varchar(80) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `semantic_view` */

insert  into `semantic_view`(`id`,`elementName`,`elementSemantic`) values (1,'ViewResource','innode'),(2,'ServerSet1',NULL),(3,'ServerSet2',NULL),(4,'ToolResource',NULL),(5,'S1',NULL),(6,'S2',NULL),(7,'S3',NULL),(8,'ToolSet1',NULL),(9,'ToolSet2',NULL),(10,'F',NULL),(11,'H1',NULL),(12,'P',NULL),(13,'H2',NULL);

/*Table structure for table `view` */

DROP TABLE IF EXISTS `view`;

CREATE TABLE `view` (
  `id` int(11) NOT NULL auto_increment,
  `elementName` varchar(80) default NULL,
  `elementParent` varchar(80) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `view` */

insert  into `view`(`id`,`elementName`,`elementParent`) values (1,'ViewResource','ServerSet1'),(2,'ViewResource','ServerSet2'),(3,'ViewResource','ToolResource'),(4,'ServerSet1','S1'),(5,'ServerSet1','S2'),(6,'ServerSet2','S2'),(7,'ServerSet2','S3'),(8,'ToolResource','ToolSet1'),(9,'ToolResource','ToolSet2'),(10,'ToolResource','F'),(11,'ToolSet1','H1'),(12,'ToolSet1','P'),(13,'ToolSet2','P'),(14,'ToolSet2','H2');

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
