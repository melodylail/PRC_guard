CREATE DATABASE `ipps`;

CREATE TABLE `UserInfo` (
  `UserName` varchar(32) NOT NULL,
  `Password` varchar(40) NOT NULL,
  `Role` int(11) NOT NULL,
  `Email` varchar(64) NOT NULL,
  `LastModifyTime` datetime NOT NULL,  
  PRIMARY KEY (`UserName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE `Report` (
  `AutoID` bigint(20) NOT NULL AUTO_INCREMENT,
  `User` varchar(32) DEFAULT NULL,
  `Content` varchar(512) DEFAULT NULL,
  `PicturePath` varchar(512) DEFAULT NULL,
  `VoicePath` varchar(512) DEFAULT NULL,
  `UpdateTime` varchar(32) DEFAULT NULL,
  PRIMARY KEY  (`AutoID`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;



INSERT INTO UserInfo (UserName, Password, Email, Role, LastModifyTime) VALUES ('admin', 'admin', 'admin@test.com', 1, now());