-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               8.0.30 - MySQL Community Server - GPL
-- Server OS:                    Win64
-- HeidiSQL Version:             12.8.0.6908
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


-- Dumping database structure for rmp
CREATE DATABASE IF NOT EXISTS `rmp` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `rmp`;

-- Dumping structure for table rmp.accounts
CREATE TABLE IF NOT EXISTS `accounts` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Username` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `DiscordID` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Password` varchar(60) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Token` smallint unsigned NOT NULL,
  `AdminLevel` tinyint unsigned NOT NULL DEFAULT '0',
  `CreatedDate` int NOT NULL,
  `LastLogin` int NOT NULL DEFAULT '0',
  `TimePlayed` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`) USING BTREE,
  UNIQUE KEY `DiscordID` (`DiscordID`),
  UNIQUE KEY `Username` (`Username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci ROW_FORMAT=DYNAMIC;

-- Dumping data for table rmp.accounts: ~1 rows (approximately)
INSERT INTO `accounts` (`ID`, `Username`, `DiscordID`, `Password`, `Token`, `AdminLevel`, `CreatedDate`, `LastLogin`, `TimePlayed`) VALUES
	(1, 'ikyy', '364683949458325516', '$2a$12$WMWtASTQ2/yMw5GpFlZ4yO9l4ySJjLKSRo//HAaiIUJsxy6FcUZae', 12345, 0, 1724263928, 1725018966, 42533),
	(3, 'rizky', '0', '$2a$12$WMWtASTQ2/yMw5GpFlZ4yO9l4ySJjLKSRo//HAaiIUJsxy6FcUZae', 12345, 0, 1724263928, 1725018963, 17642);

-- Dumping structure for table rmp.characters
CREATE TABLE IF NOT EXISTS `characters` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `AccountID` int NOT NULL,
  `FactionID` tinyint unsigned DEFAULT NULL,
  `Nickname` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `CreatedDate` int NOT NULL,
  `LastLogin` int NOT NULL DEFAULT '0',
  `TimePlayed` int NOT NULL DEFAULT '0',
  `Level` tinyint unsigned NOT NULL DEFAULT '1',
  `Gender` tinyint unsigned NOT NULL COMMENT 'boolean (0 = Male, 1 = Female)',
  `BirthDate` int NOT NULL,
  `Origin` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `Skin` smallint unsigned NOT NULL DEFAULT '1',
  `Health` float NOT NULL DEFAULT '100',
  `MaxHealth` float NOT NULL DEFAULT '100',
  `Armour` float NOT NULL DEFAULT '0',
  `Cash` mediumint NOT NULL DEFAULT '0',
  `BankBalance` int NOT NULL DEFAULT '100',
  `InjuredStatus` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'boolean (0 = Not Injured, 1 = Injured)',
  `InjuredTime` smallint unsigned NOT NULL DEFAULT '0',
  `VirtualWorld` tinyint unsigned NOT NULL DEFAULT (0),
  `Interior` tinyint unsigned NOT NULL DEFAULT '0',
  `PosX` float NOT NULL DEFAULT '0',
  `PosY` float NOT NULL DEFAULT '0',
  `PosZ` float NOT NULL DEFAULT '0',
  `PosA` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`) USING BTREE,
  UNIQUE KEY `Nickname` (`Nickname`),
  KEY `cAccountID` (`AccountID`),
  KEY `cFactionID` (`FactionID`),
  CONSTRAINT `cAccountID` FOREIGN KEY (`AccountID`) REFERENCES `accounts` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `cFactionID` FOREIGN KEY (`FactionID`) REFERENCES `faction` (`ID`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci ROW_FORMAT=DYNAMIC;

-- Dumping data for table rmp.characters: ~2 rows (approximately)
INSERT INTO `characters` (`ID`, `AccountID`, `FactionID`, `Nickname`, `CreatedDate`, `LastLogin`, `TimePlayed`, `Level`, `Gender`, `BirthDate`, `Origin`, `Skin`, `Health`, `MaxHealth`, `Armour`, `Cash`, `BankBalance`, `InjuredStatus`, `InjuredTime`, `VirtualWorld`, `Interior`, `PosX`, `PosY`, `PosZ`, `PosA`) VALUES
	(1, 1, 1, 'Rizky_Limit', 1724263928, 1725018966, 42533, 1, 0, 1120237200, 'Indonesia', 299, 63, 100, 0, 0, 100, 0, 0, 0, 0, 1787.02, -1918.88, 13.393, 236.939),
	(2, 3, NULL, 'Smith_Watson', 1724263928, 1725018963, 17642, 1, 1, 1120237200, 'Indonesia', 299, 74, 100, 0, 0, 100, 0, 0, 0, 0, 1785.35, -1920.45, 13.393, 285.915);

-- Dumping structure for table rmp.faction
CREATE TABLE IF NOT EXISTS `faction` (
  `ID` tinyint unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Dumping data for table rmp.faction: ~0 rows (approximately)
INSERT INTO `faction` (`ID`) VALUES
	(1);

-- Dumping structure for table rmp.parking
CREATE TABLE IF NOT EXISTS `parking` (
  `ID` tinyint unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Dumping data for table rmp.parking: ~0 rows (approximately)
INSERT INTO `parking` (`ID`) VALUES
	(1);

-- Dumping structure for table rmp.vehicles
CREATE TABLE IF NOT EXISTS `vehicles` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `CharacterID` int DEFAULT NULL,
  `FactionID` tinyint unsigned DEFAULT NULL,
  `ShareKeyID` int DEFAULT NULL,
  `ParkingID` tinyint unsigned DEFAULT NULL,
  `Model` smallint unsigned NOT NULL,
  `Plate` varchar(9) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `Insurance` tinyint unsigned NOT NULL DEFAULT '0',
  `Locked` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'boolean (0 = Not Lock, 1 = Locked)',
  `Handbrake` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'boolean (0 = Not Handbrake, 1 = Handbrake)',
  `Destroyed` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'boolean (0 = Not Destroy, 1 = Destroyed)',
  `Health` float NOT NULL DEFAULT '1000',
  `MaxHealth` float NOT NULL DEFAULT '1000',
  `Fuel` float NOT NULL DEFAULT '100',
  `MaxFuel` float NOT NULL DEFAULT '100',
  `UpgradeEngine` tinyint unsigned NOT NULL DEFAULT '0',
  `UpgradeBody` tinyint unsigned NOT NULL DEFAULT '0',
  `UpgradeFuel` tinyint unsigned NOT NULL DEFAULT '0',
  `DamagePanels` int unsigned NOT NULL DEFAULT (0),
  `DamageDoors` int unsigned NOT NULL DEFAULT (0),
  `DamageLights` int unsigned NOT NULL DEFAULT (0),
  `DamageTires` int unsigned NOT NULL DEFAULT (0),
  `VirtualWorld` tinyint unsigned NOT NULL,
  `Interior` tinyint unsigned NOT NULL,
  `PosX` float NOT NULL,
  `PosY` float NOT NULL,
  `PosZ` float NOT NULL,
  `PosA` float NOT NULL,
  `Color1` tinyint unsigned NOT NULL,
  `Color2` tinyint unsigned NOT NULL,
  `Siren` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'boolean (0 = Not Siren, 1 = Siren)',
  `RespawnDelay` smallint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `Plate` (`Plate`),
  KEY `CharacterID` (`CharacterID`),
  KEY `ShareKeyID` (`ShareKeyID`),
  KEY `FactionID` (`FactionID`),
  KEY `ParkingID` (`ParkingID`),
  CONSTRAINT `vCharacterID` FOREIGN KEY (`CharacterID`) REFERENCES `characters` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `vFactionID` FOREIGN KEY (`FactionID`) REFERENCES `faction` (`ID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `vParkingID` FOREIGN KEY (`ParkingID`) REFERENCES `parking` (`ID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `vShareKeyID` FOREIGN KEY (`ShareKeyID`) REFERENCES `characters` (`ID`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Dumping data for table rmp.vehicles: ~6 rows (approximately)
INSERT INTO `vehicles` (`ID`, `CharacterID`, `FactionID`, `ShareKeyID`, `ParkingID`, `Model`, `Plate`, `Insurance`, `Locked`, `Handbrake`, `Destroyed`, `Health`, `MaxHealth`, `Fuel`, `MaxFuel`, `UpgradeEngine`, `UpgradeBody`, `UpgradeFuel`, `DamagePanels`, `DamageDoors`, `DamageLights`, `DamageTires`, `VirtualWorld`, `Interior`, `PosX`, `PosY`, `PosZ`, `PosA`, `Color1`, `Color2`, `Siren`, `RespawnDelay`) VALUES
	(6, NULL, 1, NULL, NULL, 500, NULL, 0, 0, 0, 0, 1000, 1000, 100, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1794.88, -1912.23, 13.503, 143.073, 0, 0, 0, 0),
	(8, 2, NULL, 1, NULL, 500, NULL, 0, 0, 0, 0, 1000, 1000, 100, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1802.17, -1910.5, 13.513, 274.773, 0, 0, 0, 0),
	(9, 2, NULL, NULL, NULL, 510, NULL, 0, 0, 0, 0, 1000, 1000, 100, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1789.21, -1912.15, 13.004, 347.659, 0, 0, 0, 0),
	(10, 1, NULL, 2, NULL, 410, NULL, 0, 0, 0, 0, 1000, 1000, 100, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1777.52, -1893.52, 13.025, 84.182, 0, 0, 0, 0),
	(11, 1, NULL, NULL, NULL, 412, NULL, 0, 0, 0, 0, 1000, 1000, 100, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1796.5, -1919.36, 13.206, 331.124, 0, 0, 0, 0),
	(12, 1, NULL, NULL, NULL, 410, NULL, 0, 0, 0, 1, 1000, 1000, 100, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1791, -1928.9, 13.03, 140.827, 0, 0, 0, 0);

/*!40103 SET TIME_ZONE=IFNULL(@OLD_TIME_ZONE, 'system') */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
