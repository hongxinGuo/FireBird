-- MySQL dump 10.13  Distrib 8.0.19, for Win64 (x86_64)
--
-- Host: localhost    Database: smartstockdata
-- ------------------------------------------------------
-- Server version	8.0.19

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `choicedstocks`
--

DROP TABLE IF EXISTS `choicedstocks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choicedstocks` (
  `Market` int DEFAULT NULL,
  `StockCode` varchar(8) DEFAULT NULL,
  KEY `StockCode` (`StockCode`),
  KEY `Market` (`Market`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `crweberindex`
--

DROP TABLE IF EXISTS `crweberindex`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `crweberindex` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Day` int DEFAULT '0',
  `VLCC_TD1` decimal(10,3) DEFAULT '0.000',
  `VLCC_TD2` decimal(10,3) DEFAULT '0.000',
  `VLCC_TD3C` decimal(10,3) DEFAULT '0.000',
  `VLCC_TD15` decimal(10,3) DEFAULT '0.000',
  `VLCC_USGSPORE` decimal(30,3) DEFAULT '0.000' COMMENT '单位：万美元',
  `SUEZMAX_TD5` decimal(10,3) DEFAULT '0.000',
  `SUEZMAX_TD6` decimal(10,3) DEFAULT '0.000',
  `SUEZMAX_TD20` decimal(10,3) DEFAULT '0.000',
  `SUEZMAX_CBSUSG` decimal(10,3) DEFAULT '0.000',
  `AFRAMAX_TD7` decimal(10,3) DEFAULT '0.000',
  `AFRAMAX_TD8` decimal(10,3) DEFAULT '0.000',
  `AFRAMAX_TD9` decimal(10,3) DEFAULT '0.000',
  `AFRAMAX_TD19` decimal(10,3) DEFAULT '0.000',
  `PANAMAX_TD12` decimal(10,3) DEFAULT '0.000',
  `PANAMAX_TD21` decimal(10,3) DEFAULT '0.000',
  `CPP_TC1` decimal(10,3) DEFAULT '0.000',
  `CPP_TC2` decimal(10,3) DEFAULT '0.000',
  `CPP_TC3` decimal(10,3) DEFAULT '0.000',
  `CPP_TC4` decimal(10,3) DEFAULT '0.000',
  `CPP_TC5` decimal(10,3) DEFAULT '0.000',
  `CPP_TC14` decimal(10,3) DEFAULT '0.000',
  `CPP_USGCBS` decimal(10,3) DEFAULT '0.000',
  `VLCC_TC_1YEAR` decimal(10,3) DEFAULT '0.000',
  `VLCC_TC_3YEAR` decimal(10,3) DEFAULT '0.000',
  `SUEZMAX_TC_1YEAR` decimal(10,3) DEFAULT '0.000',
  `SUEZMAX_TC_3YEAR` decimal(10,3) DEFAULT '0.000',
  `AFRAMAX_TC_1YEAR` decimal(10,3) DEFAULT '0.000',
  `AFRAMAX_TC_3YEAR` decimal(10,3) DEFAULT '0.000',
  `PANAMAX_TC_1YEAR` decimal(10,3) DEFAULT '0.000',
  `PANAMAX_TC_3YEAR` decimal(10,3) DEFAULT '0.000',
  `MR_TC_1YEAR` decimal(10,3) DEFAULT '0.000',
  `MR_TC_3YEAR` decimal(10,3) DEFAULT '0.000',
  `HANDY_TC_1YEAR` decimal(10,3) DEFAULT '0.000',
  `HANDY_TC_3YEAR` decimal(10,3) DEFAULT '0.000',
  PRIMARY KEY (`ID`),
  KEY `TD1` (`VLCC_TD1`) /*!80000 INVISIBLE */,
  KEY `TD2` (`VLCC_TD2`) /*!80000 INVISIBLE */,
  KEY `TD3C` (`VLCC_TD3C`) /*!80000 INVISIBLE */,
  KEY `TD5` (`SUEZMAX_TD5`) /*!80000 INVISIBLE */,
  KEY `TD6` (`SUEZMAX_TD6`) /*!80000 INVISIBLE */,
  KEY `TD7` (`AFRAMAX_TD7`) /*!80000 INVISIBLE */,
  KEY `TD8` (`AFRAMAX_TD8`) /*!80000 INVISIBLE */,
  KEY `TD9` (`AFRAMAX_TD9`) /*!80000 INVISIBLE */,
  KEY `TD12` (`PANAMAX_TD12`) /*!80000 INVISIBLE */,
  KEY `TD15` (`VLCC_TD15`) /*!80000 INVISIBLE */,
  KEY `TD19` (`AFRAMAX_TD19`) /*!80000 INVISIBLE */,
  KEY `TD20` (`SUEZMAX_TD20`) /*!80000 INVISIBLE */,
  KEY `TD21` (`PANAMAX_TD21`) /*!80000 INVISIBLE */,
  KEY `VLCC_USGSPORT` (`VLCC_USGSPORE`) /*!80000 INVISIBLE */,
  KEY `SUEZMAX_CBSUSG` (`SUEZMAX_CBSUSG`) /*!80000 INVISIBLE */,
  KEY `TC1` (`CPP_TC1`) /*!80000 INVISIBLE */,
  KEY `TC2` (`CPP_TC2`) /*!80000 INVISIBLE */,
  KEY `TC3` (`CPP_TC3`) /*!80000 INVISIBLE */,
  KEY `TC4` (`CPP_TC4`) /*!80000 INVISIBLE */,
  KEY `TC5` (`CPP_TC5`) /*!80000 INVISIBLE */,
  KEY `TC14` (`CPP_TC14`),
  KEY `CPP_USGCBS` (`CPP_USGCBS`),
  KEY `time` (`Day`)
) ENGINE=InnoDB AUTO_INCREMENT=4224 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dayline`
--

DROP TABLE IF EXISTS `dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Day` int DEFAULT '19900101',
  `Market` smallint DEFAULT '0',
  `StockCode` varchar(10) DEFAULT ' ',
  `StockName` varchar(12) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `TIME` (`Day`,`UpDownRate`),
  KEY `STOCKCODE` (`StockCode`,`Day`)
) ENGINE=InnoDB AUTO_INCREMENT=13065294 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daylineinfo`
--

DROP TABLE IF EXISTS `daylineinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `daylineinfo` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Day` int DEFAULT '19900101',
  `Market` smallint DEFAULT '0',
  `StockCode` varchar(10) DEFAULT ' ',
  `TransactionNumber` decimal(12,0) DEFAULT '0',
  `TransactionNumberBelow5000` decimal(12,0) DEFAULT '0',
  `TransactionNumberBelow50000` decimal(12,0) DEFAULT '0',
  `TransactionNumberBelow200000` decimal(12,0) DEFAULT '0',
  `TransactionNumberAbove200000` decimal(12,0) DEFAULT '0',
  `OrdinaryBuyVolume` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolume` decimal(15,0) DEFAULT '0',
  `UnknownVolume` decimal(15,0) DEFAULT '0',
  `AttackBuyVolume` decimal(15,0) DEFAULT '0',
  `AttackSellVolume` decimal(15,0) DEFAULT '0',
  `StrongBuyVolume` decimal(15,0) DEFAULT '0',
  `StrongSellVolume` decimal(15,0) DEFAULT '0',
  `CancelBuyVolume` decimal(15,0) DEFAULT '0',
  `CancelSellVolume` decimal(15,0) DEFAULT '0',
  `AttackBuyBelow50000` decimal(15,0) DEFAULT '0',
  `AttackBuyBelow200000` decimal(15,0) DEFAULT '0',
  `AttackBuyAbove200000` decimal(15,0) DEFAULT '0',
  `AttackSellBelow50000` decimal(15,0) DEFAULT '0',
  `AttackSellBelow200000` decimal(15,0) DEFAULT '0',
  `AttackSellAbove200000` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `STOCKCODE` (`StockCode`,`Day`),
  KEY `TIME` (`Day`)
) ENGINE=InnoDB AUTO_INCREMENT=13333067 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `options`
--

DROP TABLE IF EXISTS `options`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `options` (
  `RelativeStrongStartDay` int NOT NULL DEFAULT '19900101',
  `RelativeStrongEndDay` int DEFAULT '19900101',
  `LastLogInDay` int DEFAULT '19900101',
  PRIMARY KEY (`RelativeStrongStartDay`),
  KEY `StartDay` (`RelativeStrongStartDay`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `potendailybriefing`
--

DROP TABLE IF EXISTS `potendailybriefing`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `potendailybriefing` (
  `Id` int NOT NULL AUTO_INCREMENT,
  `Day` int DEFAULT '0',
  `VLCC_TD3C` decimal(10,3) DEFAULT '0.000' COMMENT 'AG-FE',
  `SUEZMAX_TD20` decimal(10,3) DEFAULT '0.000' COMMENT 'WAF-UKC',
  `AFRAMAX_TD9` decimal(10,3) DEFAULT '0.000' COMMENT 'CAR-USG',
  `PANAMAX_TD21` decimal(10,3) DEFAULT '0.000' COMMENT 'CAR-USG',
  `LR2_TC1` decimal(10,3) DEFAULT '0.000' COMMENT 'AG-FE',
  `LR1_TC5` decimal(10,3) DEFAULT '0.000' COMMENT 'AG-FE',
  `MR_TC2` decimal(10,3) DEFAULT '0.000' COMMENT 'USG-UKC',
  `MR_TC14` decimal(10,3) DEFAULT '0.000' COMMENT 'UKC-USAG',
  `VLCC_TC1YEAR` decimal(10,3) DEFAULT '0.000',
  `SUEZMAX_TC1YEAR` decimal(10,3) DEFAULT '0.000',
  `AFRAMAX_TC1YEAR` decimal(10,3) DEFAULT '0.000',
  `LR2_TC1YEAR` decimal(10,3) DEFAULT '0.000',
  `LR1_TC1YEAR` decimal(10,3) DEFAULT '0.000',
  `MR_TC1YEAR` decimal(10,3) DEFAULT '0.000',
  `HANDYMAX_TC1YEAR` decimal(10,3) DEFAULT '0.000',
  `VLCC_NEWBUILD` decimal(10,3) DEFAULT '0.000',
  `SUEZMAX_NEWBUILD` decimal(10,3) DEFAULT '0.000',
  `AFRAMAX_NEWBUILD` decimal(10,3) DEFAULT '0.000',
  `LR2_NEWBUILD` decimal(10,3) DEFAULT '0.000',
  `LR1_NEWBUILD` decimal(10,3) DEFAULT '0.000',
  `MR_NEWBUILD` decimal(10,3) DEFAULT '0.000',
  `HANDYMAX_NEWBUILD` decimal(10,3) DEFAULT '0.000',
  `VLCC_5YEAROLD` decimal(10,3) DEFAULT '0.000',
  `SUEZMAX_5YEAROLD` decimal(10,3) DEFAULT '0.000',
  `AFRAMAX_5YEAROLD` decimal(10,3) DEFAULT '0.000',
  `LR2_5YEAROLD` decimal(10,3) DEFAULT '0.000',
  `LR1_5YEAROLD` decimal(10,3) DEFAULT '0.000',
  `MR_5YEAROLD` decimal(10,3) DEFAULT '0.000',
  `HANDYMAX_5YEAROLD` decimal(10,3) DEFAULT '0.000',
  `HANDYUSFLAG_USG_USAC` decimal(30,3) DEFAULT '0.000',
  `LNG_160M3WESTWEEKLY` decimal(30,3) DEFAULT '0.000',
  `LNG_160M3WESTMONTHLY` decimal(30,3) DEFAULT '0.000',
  `LNG_138M3WESTWEEKLY` decimal(30,3) DEFAULT '0.000',
  `LNG_138M3WESTMONTHLY` decimal(30,3) DEFAULT '0.000',
  `LNG_160M3EASTWEEKLY` decimal(30,3) DEFAULT '0.000',
  `LNG_160M3EASTMONTHLY` decimal(30,3) DEFAULT '0.000',
  `LNG_138M3EASTWEEKLY` decimal(30,3) DEFAULT '0.000',
  `LNG_138M3EASTMONTHLY` decimal(30,3) DEFAULT '0.000',
  `LPG_VLGC44LASTSPOTRATE` decimal(30,3) DEFAULT '0.000' COMMENT '$/mt',
  `LPG_VLGC44SPOTTCERATE` decimal(30,3) DEFAULT '0.000' COMMENT '$/day',
  PRIMARY KEY (`Id`),
  KEY `ID` (`Id`) /*!80000 INVISIBLE */,
  KEY `Day` (`Day`)
) ENGINE=InnoDB AUTO_INCREMENT=433 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='Poten Daily Briefing for shipping Index.';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `realtimedata`
--

DROP TABLE IF EXISTS `realtimedata`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `realtimedata` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Time` decimal(15,0) DEFAULT '0',
  `Market` int DEFAULT '0',
  `StockName` varchar(12) DEFAULT '0',
  `StockCode` varchar(8) DEFAULT '0',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `New` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `Stroke` decimal(10,0) DEFAULT '0',
  `PBuy5` decimal(10,3) DEFAULT '0.000',
  `VBuy5` decimal(15,0) DEFAULT '0',
  `PBuy4` decimal(10,3) DEFAULT '0.000',
  `VBuy4` decimal(15,0) DEFAULT '0',
  `PBuy3` decimal(10,3) DEFAULT '0.000',
  `VBuy3` decimal(15,0) DEFAULT '0',
  `PBuy2` decimal(10,3) DEFAULT '0.000',
  `VBuy2` decimal(15,0) DEFAULT '0',
  `PBuy1` decimal(10,3) DEFAULT '0.000',
  `VBuy1` decimal(15,0) DEFAULT '0',
  `PSell1` decimal(10,3) DEFAULT '0.000',
  `VSell1` decimal(15,0) DEFAULT '0',
  `PSell2` decimal(10,3) DEFAULT '0.000',
  `VSell2` decimal(15,0) DEFAULT '0',
  `PSell3` decimal(10,3) DEFAULT '0.000',
  `VSell3` decimal(15,0) DEFAULT '0',
  `PSell4` decimal(10,3) DEFAULT '0.000',
  `VSell4` decimal(15,0) DEFAULT '0',
  `PSell5` decimal(10,3) DEFAULT '0.000',
  `VSell5` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  KEY `StockCode` (`StockCode`),
  KEY `Time` (`Time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stockcode`
--

DROP TABLE IF EXISTS `stockcode`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stockcode` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Counter` int DEFAULT '0',
  `StockType` smallint DEFAULT '0',
  `StockCode` varchar(8) DEFAULT ' ',
  `StockName` varchar(12) DEFAULT ' ',
  `DayLineStartDay` int DEFAULT '29900101',
  `DayLineEndDay` int DEFAULT '19900101',
  `IPOed` int DEFAULT '128',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `STOCKCODE` (`StockCode`)
) ENGINE=InnoDB AUTO_INCREMENT=4289698 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `today`
--

DROP TABLE IF EXISTS `today`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `today` (
  `Id` int NOT NULL AUTO_INCREMENT,
  `Day` int DEFAULT '0',
  `Market` smallint DEFAULT '0',
  `StockCode` varchar(10) DEFAULT ' ',
  `StockName` varchar(12) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `TransactionNumber` decimal(12,0) DEFAULT '0',
  `TransactionNumberBelow5000` decimal(12,0) DEFAULT '0',
  `TransactionNumberBelow50000` decimal(12,0) DEFAULT '0',
  `TransactionNumberBelow200000` decimal(12,0) DEFAULT '0',
  `TransactionNumberAbove200000` decimal(12,0) DEFAULT '0',
  `OrdinaryBuyVolume` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolume` decimal(15,0) DEFAULT '0',
  `UnknownVolume` decimal(15,0) DEFAULT '0',
  `AttackBuyVolume` decimal(15,0) DEFAULT '0',
  `AttackSellVolume` decimal(15,0) DEFAULT '0',
  `StrongBuyVolume` decimal(15,0) DEFAULT '0',
  `StrongSellVolume` decimal(15,0) DEFAULT '0',
  `CancelBuyVolume` decimal(15,0) DEFAULT '0',
  `CancelSellVolume` decimal(15,0) DEFAULT '0',
  `AttackBuyBelow50000` decimal(15,0) DEFAULT '0',
  `AttackBuyBelow200000` decimal(15,0) DEFAULT '0',
  `AttackBuyAbove200000` decimal(15,0) DEFAULT '0',
  `AttackSellBelow50000` decimal(15,0) DEFAULT '0',
  `AttackSellBelow200000` decimal(15,0) DEFAULT '0',
  `AttackSellAbove200000` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`Id`),
  UNIQUE KEY `Id_UNIQUE` (`Id`),
  KEY `time` (`Day`)
) ENGINE=InnoDB AUTO_INCREMENT=17630973 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2020-03-21  9:08:20
