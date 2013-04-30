/*
Navicat MySQL Data Transfer

Source Server         : policy
Source Server Version : 50022
Source Host           : localhost:3308
Source Database       : fileservices

Target Server Type    : MYSQL
Target Server Version : 50022
File Encoding         : 65001

Date: 2013-04-25 15:46:18
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `activity`
-- ----------------------------
DROP TABLE IF EXISTS `activity`;
CREATE TABLE `activity` (
`id`  int(11) NOT NULL AUTO_INCREMENT ,
`elementName`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`elementParent`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=latin1 COLLATE=latin1_swedish_ci
/*!50003 AUTO_INCREMENT=4 */

;

-- ----------------------------
-- Table structure for `context`
-- ----------------------------
DROP TABLE IF EXISTS `context`;
CREATE TABLE `context` (
`id`  int(11) NOT NULL AUTO_INCREMENT ,
`elementName`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`elementParent`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=latin1 COLLATE=latin1_swedish_ci
/*!50003 AUTO_INCREMENT=9 */

;

-- ----------------------------
-- Table structure for `measure`
-- ----------------------------
DROP TABLE IF EXISTS `measure`;
CREATE TABLE `measure` (
`id`  int(11) NOT NULL AUTO_INCREMENT ,
`elementName`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`elementParent`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=latin1 COLLATE=latin1_swedish_ci
/*!50003 AUTO_INCREMENT=3 */

;

-- ----------------------------
-- Table structure for `policy`
-- ----------------------------
DROP TABLE IF EXISTS `policy`;
CREATE TABLE `policy` (
`id`  int(11) NOT NULL AUTO_INCREMENT ,
`type`  int(11) NULL DEFAULT NULL ,
`context`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`role`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`view`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`activity`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`measure`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`organization`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=latin1 COLLATE=latin1_swedish_ci
/*!50003 AUTO_INCREMENT=115 */

;

-- ----------------------------
-- Table structure for `result_conflict`
-- ----------------------------
DROP TABLE IF EXISTS `result_conflict`;
CREATE TABLE `result_conflict` (
`id`  int(11) NOT NULL AUTO_INCREMENT ,
`policyAmount`  int(11) NULL DEFAULT NULL ,
`conflictAmount`  int(11) NULL DEFAULT NULL ,
`unrelation`  int(11) NULL DEFAULT NULL ,
`syntaxConflict`  int(11) NULL DEFAULT NULL ,
`inclusiveConflict`  int(11) NULL DEFAULT NULL ,
`partialConflict`  int(11) NULL DEFAULT NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=latin1 COLLATE=latin1_swedish_ci
/*!50003 AUTO_INCREMENT=2 */

;

-- ----------------------------
-- Table structure for `role`
-- ----------------------------
DROP TABLE IF EXISTS `role`;
CREATE TABLE `role` (
`id`  int(11) NOT NULL AUTO_INCREMENT ,
`elementName`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`elementParent`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=latin1 COLLATE=latin1_swedish_ci
/*!50003 AUTO_INCREMENT=67 */

;

-- ----------------------------
-- Table structure for `semantic_activity`
-- ----------------------------
DROP TABLE IF EXISTS `semantic_activity`;
CREATE TABLE `semantic_activity` (
`id`  int(11) NOT NULL AUTO_INCREMENT ,
`elementName`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`elementSemantic`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=latin1 COLLATE=latin1_swedish_ci
/*!50003 AUTO_INCREMENT=4 */

;

-- ----------------------------
-- Table structure for `semantic_context`
-- ----------------------------
DROP TABLE IF EXISTS `semantic_context`;
CREATE TABLE `semantic_context` (
`id`  int(11) NOT NULL AUTO_INCREMENT ,
`elementName`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`elementSemantic`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=latin1 COLLATE=latin1_swedish_ci
/*!50003 AUTO_INCREMENT=9 */

;

-- ----------------------------
-- Table structure for `semantic_mapping`
-- ----------------------------
DROP TABLE IF EXISTS `semantic_mapping`;
CREATE TABLE `semantic_mapping` (
`id`  int(11) NOT NULL AUTO_INCREMENT ,
`elementName`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`elementSemantic`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=latin1 COLLATE=latin1_swedish_ci
/*!50003 AUTO_INCREMENT=1 */

;

-- ----------------------------
-- Table structure for `semantic_measure`
-- ----------------------------
DROP TABLE IF EXISTS `semantic_measure`;
CREATE TABLE `semantic_measure` (
`id`  int(11) NOT NULL AUTO_INCREMENT ,
`elementName`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`elementSemantic`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`type`  int(11) NULL DEFAULT NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=latin1 COLLATE=latin1_swedish_ci
/*!50003 AUTO_INCREMENT=3 */

;

-- ----------------------------
-- Table structure for `semantic_role`
-- ----------------------------
DROP TABLE IF EXISTS `semantic_role`;
CREATE TABLE `semantic_role` (
`id`  int(11) NOT NULL AUTO_INCREMENT ,
`elementName`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`elementSemantic`  varchar(100) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=latin1 COLLATE=latin1_swedish_ci
/*!50003 AUTO_INCREMENT=66 */

;

-- ----------------------------
-- Table structure for `semantic_view`
-- ----------------------------
DROP TABLE IF EXISTS `semantic_view`;
CREATE TABLE `semantic_view` (
`id`  int(11) NOT NULL AUTO_INCREMENT ,
`elementName`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`elementSemantic`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=latin1 COLLATE=latin1_swedish_ci
/*!50003 AUTO_INCREMENT=36 */

;

-- ----------------------------
-- Table structure for `view`
-- ----------------------------
DROP TABLE IF EXISTS `view`;
CREATE TABLE `view` (
`id`  int(11) NOT NULL AUTO_INCREMENT ,
`elementName`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
`elementParent`  varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL ,
PRIMARY KEY (`id`)
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=latin1 COLLATE=latin1_swedish_ci
/*!50003 AUTO_INCREMENT=116 */

;

-- ----------------------------
-- Auto increment value for `activity`
-- ----------------------------
ALTER TABLE `activity` AUTO_INCREMENT=4;

-- ----------------------------
-- Auto increment value for `context`
-- ----------------------------
ALTER TABLE `context` AUTO_INCREMENT=9;

-- ----------------------------
-- Auto increment value for `measure`
-- ----------------------------
ALTER TABLE `measure` AUTO_INCREMENT=3;

-- ----------------------------
-- Auto increment value for `policy`
-- ----------------------------
ALTER TABLE `policy` AUTO_INCREMENT=115;

-- ----------------------------
-- Auto increment value for `result_conflict`
-- ----------------------------
ALTER TABLE `result_conflict` AUTO_INCREMENT=2;

-- ----------------------------
-- Auto increment value for `role`
-- ----------------------------
ALTER TABLE `role` AUTO_INCREMENT=67;

-- ----------------------------
-- Auto increment value for `semantic_activity`
-- ----------------------------
ALTER TABLE `semantic_activity` AUTO_INCREMENT=4;

-- ----------------------------
-- Auto increment value for `semantic_context`
-- ----------------------------
ALTER TABLE `semantic_context` AUTO_INCREMENT=9;

-- ----------------------------
-- Auto increment value for `semantic_mapping`
-- ----------------------------
ALTER TABLE `semantic_mapping` AUTO_INCREMENT=1;

-- ----------------------------
-- Auto increment value for `semantic_measure`
-- ----------------------------
ALTER TABLE `semantic_measure` AUTO_INCREMENT=3;

-- ----------------------------
-- Auto increment value for `semantic_role`
-- ----------------------------
ALTER TABLE `semantic_role` AUTO_INCREMENT=66;

-- ----------------------------
-- Auto increment value for `semantic_view`
-- ----------------------------
ALTER TABLE `semantic_view` AUTO_INCREMENT=36;

-- ----------------------------
-- Auto increment value for `view`
-- ----------------------------
ALTER TABLE `view` AUTO_INCREMENT=116;
