CREATE DATABASE `mops`;

CREATE TABLE `UserInfo` (
  `UserName` varchar(32) NOT NULL,
  `Password` varchar(40) NOT NULL,
  `Role` int(11) NOT NULL,
  `Email` varchar(64) NOT NULL,
  `LastModifyTime` datetime NOT NULL,  
  PRIMARY KEY (`UserName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE `MacInfo` (
  `Mac` varchar(32) DEFAULT NULL,
  `User` varchar(512) DEFAULT NULL,
  `Name` varchar(512) DEFAULT NULL,
  PRIMARY KEY  (`Mac`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE `Report` (
  `Uuid` binary(16) NOT NULL,
  `User` varchar(512) DEFAULT NULL,
  `Name` varchar(512) DEFAULT NULL,
  `Room` varchar(128) NOT NULL,
  `Msg` varchar(1024) DEFAULT NULL,
  `Time` datetime NOT NULL, 
  PRIMARY KEY  (`Uuid`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE `Device` (
  `Id` varchar(64)  NOT NULL,
  `Name` varchar(512) NOT NULL,
  `Type` varchar(128) NOT NULL,
  `Status` int(11) NOT NULL,
  `Channel` varchar(128) NOT NULL,
  `NetInterface` varchar(128) NOT NULL,
  `LastLiveTime` datetime NOT NULL,
  PRIMARY KEY  (`id`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;



INSERT INTO UserInfo (UserName, Password, Email, Role, LastModifyTime) VALUES ('admin', 'admin', 'admin@test.com', 1, now());

INSERT INTO Device (Id, Name, Type, Status, Channel,NetInterface,LastLiveTime) VALUES ('123456', 'capter1', 'asus pad', 1, "1", "wlan0", now());