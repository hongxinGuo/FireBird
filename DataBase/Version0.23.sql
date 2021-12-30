-- MySQL dump 10.13  Distrib 8.0.27, for Win64 (x86_64)
--
-- Host: localhost    Database: worldmarket_test
-- ------------------------------------------------------
-- Server version	8.0.27

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
-- Table structure for table `choiced_crypto`
--

DROP TABLE IF EXISTS `choiced_crypto`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choiced_crypto` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `choiced_forex`
--

DROP TABLE IF EXISTS `choiced_forex`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choiced_forex` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `choiced_stock`
--

DROP TABLE IF EXISTS `choiced_stock`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choiced_stock` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `country_list`
--

DROP TABLE IF EXISTS `country_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `country_list` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code2` varchar(45) DEFAULT ' ',
  `Code3` varchar(45) DEFAULT ' ',
  `CodeNo` varchar(45) DEFAULT ' ',
  `Country` varchar(100) DEFAULT ' ',
  `Currency` varchar(45) DEFAULT ' ',
  `CurrencyCode` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Code` (`Code2`) /*!80000 INVISIBLE */,
  KEY `CurrencyCode` (`CurrencyCode`)
) ENGINE=InnoDB AUTO_INCREMENT=4546 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `crypto_dayline`
--

DROP TABLE IF EXISTS `crypto_dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `crypto_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19800101',
  `Exchange` varchar(50) DEFAULT ' ',
  `Symbol` varchar(50) DEFAULT ' ',
  `DisplaySymbol` varchar(45) DEFAULT '',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`,`Date`) /*!80000 INVISIBLE */,
  KEY `Date` (`Symbol`,`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=1611225 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `economic_calendar`
--

DROP TABLE IF EXISTS `economic_calendar`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `economic_calendar` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Time` bigint DEFAULT '0',
  `Country` varchar(45) DEFAULT ' ',
  `Event` varchar(100) DEFAULT ' ',
  `Impact` varchar(45) DEFAULT ' ',
  `Actual` decimal(10,3) DEFAULT '0.000',
  `estimate` decimal(10,3) DEFAULT '0.000',
  `Prev` decimal(10,3) DEFAULT '0.000',
  `Unit` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3460 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `eps_surprise`
--

DROP TABLE IF EXISTS `eps_surprise`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `eps_surprise` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT ' ',
  `Date` int DEFAULT '0',
  `Actual` double DEFAULT '0',
  `Estimate` double DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`),
  KEY `Date` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=107582 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_crypto_exchange`
--

DROP TABLE IF EXISTS `finnhub_crypto_exchange`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_crypto_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` varchar(45) DEFAULT '',
  `Name` varchar(100) DEFAULT '',
  `Mic` varchar(45) DEFAULT '',
  `TimeZone` varchar(45) DEFAULT '',
  `Hour` varchar(45) DEFAULT '',
  `CloseDate` varchar(45) DEFAULT '',
  `Country` varchar(45) DEFAULT '',
  `Source` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Exchange` (`Code`)
) ENGINE=InnoDB AUTO_INCREMENT=726 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_crypto_symbol`
--

DROP TABLE IF EXISTS `finnhub_crypto_symbol`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_crypto_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Description` varchar(100) DEFAULT ' ',
  `DisplaySymbol` varchar(100) DEFAULT ' ',
  `Exchange` varchar(100) DEFAULT ' ',
  `Symbol` varchar(100) DEFAULT ' ',
  `DayLineStartDate` int DEFAULT '29900101',
  `DayLineEndDate` int DEFAULT '19800101',
  `IPOStatus` int DEFAULT '128',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`) /*!80000 INVISIBLE */,
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=8367 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_forex_exchange`
--

DROP TABLE IF EXISTS `finnhub_forex_exchange`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_forex_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` varchar(45) DEFAULT '',
  `Name` varchar(100) DEFAULT '',
  `Mic` varchar(45) DEFAULT '',
  `TimeZone` varchar(45) DEFAULT '',
  `Hour` varchar(45) DEFAULT '',
  `CloseDate` varchar(45) DEFAULT '',
  `Country` varchar(45) DEFAULT '',
  `Source` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Exchange` (`Code`)
) ENGINE=InnoDB AUTO_INCREMENT=2032 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_forex_symbol`
--

DROP TABLE IF EXISTS `finnhub_forex_symbol`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_forex_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Description` varchar(100) DEFAULT ' ',
  `Exchange` varchar(100) DEFAULT ' ',
  `Symbol` varchar(100) DEFAULT ' ',
  `DisplaySymbol` varchar(100) DEFAULT ' ',
  `DayLineStartDate` int DEFAULT '29900101',
  `DayLineEndDate` int DEFAULT '19800101',
  `IPOStatus` int DEFAULT '128',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=7177 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_profile`
--

DROP TABLE IF EXISTS `finnhub_profile`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_profile` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Address` varchar(100) DEFAULT ' ',
  `City` varchar(45) DEFAULT ' ',
  `Country` varchar(45) DEFAULT ' ',
  `Currency` varchar(45) DEFAULT ' ',
  `cusip` varchar(45) DEFAULT ' ',
  `sedol` varchar(45) DEFAULT ' ',
  `EmployeeTotal` int DEFAULT '0',
  `Exchange` varchar(100) DEFAULT ' ',
  `ggroup` varchar(45) DEFAULT ' ',
  `gind` varchar(45) DEFAULT ' ',
  `gsector` varchar(45) DEFAULT ' ',
  `gsubind` varchar(45) DEFAULT ' ',
  `IPODate` varchar(20) DEFAULT ' ',
  `isin` varchar(45) DEFAULT ' ',
  `MarketCapitalization` decimal(25,6) DEFAULT '0.000000',
  `naics` varchar(45) DEFAULT ' ',
  `naicsNationalIndustry` varchar(45) DEFAULT ' ',
  `naicsSector` varchar(45) DEFAULT ' ',
  `naicsSubsector` varchar(45) DEFAULT ' ',
  `Name` varchar(100) DEFAULT ' ',
  `Phone` varchar(100) DEFAULT ' ',
  `ShareOutstanding` decimal(20,6) DEFAULT '0.000000',
  `state` varchar(45) DEFAULT ' ',
  `Ticker` varchar(45) DEFAULT ' ',
  `WebURL` varchar(150) DEFAULT ' ',
  `Logo` varchar(110) DEFAULT ' ',
  `FinnhubIndustry` varchar(100) DEFAULT ' ',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_exchange`
--

DROP TABLE IF EXISTS `finnhub_stock_exchange`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` varchar(45) DEFAULT '',
  `Name` varchar(100) DEFAULT '',
  `Mic` varchar(45) DEFAULT '',
  `TimeZone` varchar(45) DEFAULT '',
  `Hour` varchar(45) DEFAULT '',
  `CloseDate` varchar(45) DEFAULT '',
  `Country` varchar(45) DEFAULT '',
  `Source` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2095 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_exchange_old`
--

DROP TABLE IF EXISTS `finnhub_stock_exchange_old`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_exchange_old` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `IsActive` tinyint DEFAULT '0',
  `Code` varchar(45) DEFAULT NULL,
  `Name` varchar(100) DEFAULT '',
  `Mic` varchar(45) DEFAULT '',
  `TimeZone` varchar(45) DEFAULT '',
  `Hour` varchar(45) DEFAULT '',
  `CloseDate` varchar(45) DEFAULT '',
  `Country` varchar(45) DEFAULT '',
  `Source` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=69 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_symbol`
--

DROP TABLE IF EXISTS `finnhub_stock_symbol`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Currency` varchar(45) DEFAULT ' ',
  `Description` varchar(200) DEFAULT ' ',
  `DisplaySymbol` varchar(45) DEFAULT ' ',
  `Figi` varchar(45) DEFAULT ' ',
  `Mic` varchar(45) DEFAULT ' ',
  `Symbol` varchar(45) DEFAULT ' ',
  `Type` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `forex_dayline`
--

DROP TABLE IF EXISTS `forex_dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `forex_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19800101',
  `Exchange` varchar(50) DEFAULT ' ',
  `Symbol` varchar(50) DEFAULT ' ',
  `DisplaySymbol` varchar(45) DEFAULT '',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`) /*!80000 INVISIBLE */,
  KEY `Symbol` (`Symbol`,`Date`),
  KEY `Date` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=230423 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `insider_transaction`
--

DROP TABLE IF EXISTS `insider_transaction`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `insider_transaction` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `PersonName` varchar(200) DEFAULT NULL,
  `Share` decimal(15,0) DEFAULT NULL,
  `Change` decimal(15,0) DEFAULT NULL,
  `FilingDate` int DEFAULT NULL,
  `TransactionDate` int DEFAULT NULL,
  `TransactionCode` varchar(5) DEFAULT NULL,
  `TransactionPrice` double DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`) /*!80000 INVISIBLE */,
  KEY `TransactionDate` (`TransactionDate`) /*!80000 INVISIBLE */,
  KEY `PersonName` (`PersonName`)
) ENGINE=InnoDB AUTO_INCREMENT=13952 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `naics_industry`
--

DROP TABLE IF EXISTS `naics_industry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `naics_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Naics` varchar(45) DEFAULT ' ',
  `NationalIndustry` varchar(45) DEFAULT ' ',
  `Sector` varchar(45) DEFAULT ' ',
  `SubSector` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  KEY `NAICS` (`Naics`) /*!80000 INVISIBLE */,
  KEY `Industry` (`NationalIndustry`,`Sector`,`SubSector`)
) ENGINE=InnoDB AUTO_INCREMENT=2018 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='NAICS行业标准';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sic_industry`
--

DROP TABLE IF EXISTS `sic_industry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `sic_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` int DEFAULT '0',
  `Industry` varchar(45) DEFAULT ' ',
  `Sector` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  KEY `SICCode` (`Code`) /*!80000 INVISIBLE */,
  KEY `Industry` (`Industry`,`Sector`)
) ENGINE=InnoDB AUTO_INCREMENT=2022 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='SIC行业标准';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stock_dayline`
--

DROP TABLE IF EXISTS `stock_dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stock_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19800101',
  `Exchange` varchar(100) DEFAULT ' ',
  `Symbol` varchar(45) DEFAULT ' ',
  `DisplaySymbol` varchar(45) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Date` (`Symbol`,`Date`) /*!80000 INVISIBLE */,
  KEY `Symbol` (`Symbol`,`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=9029114 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stock_profile`
--

DROP TABLE IF EXISTS `stock_profile`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stock_profile` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(20) DEFAULT ' ',
  `ExchangeCode` varchar(3) DEFAULT 'US',
  `Description` varchar(200) DEFAULT ' ',
  `DisplaySymbol` varchar(20) DEFAULT ' ',
  `Type` varchar(25) DEFAULT ' ',
  `Mic` varchar(20) DEFAULT ' ',
  `Figi` varchar(20) DEFAULT ' ',
  `Currency` varchar(20) DEFAULT ' ',
  `Address` varchar(100) DEFAULT ' ',
  `City` varchar(20) DEFAULT ' ',
  `Country` varchar(20) DEFAULT ' ',
  `cusip` varchar(20) DEFAULT ' ',
  `sedol` varchar(20) DEFAULT ' ',
  `EmployeeTotal` int DEFAULT '0',
  `ListedExchange` varchar(100) DEFAULT ' ' COMMENT '所上市的交易所名称',
  `ggroup` varchar(45) DEFAULT ' ',
  `gind` varchar(45) DEFAULT ' ',
  `gsector` varchar(45) DEFAULT ' ',
  `gsubind` varchar(45) DEFAULT ' ',
  `IPODate` varchar(20) DEFAULT ' ',
  `isin` varchar(45) DEFAULT ' ',
  `MarketCapitalization` decimal(25,6) DEFAULT '0.000000',
  `naics` varchar(45) DEFAULT ' ',
  `naicsNationalIndustry` varchar(100) DEFAULT ' ',
  `naicsSector` varchar(100) DEFAULT ' ',
  `naicsSubsector` varchar(100) DEFAULT ' ',
  `Name` varchar(200) DEFAULT ' ',
  `Phone` varchar(100) DEFAULT ' ',
  `ShareOutstanding` decimal(20,6) DEFAULT '0.000000',
  `state` varchar(45) DEFAULT ' ',
  `Ticker` varchar(45) DEFAULT ' ',
  `WebURL` varchar(150) DEFAULT ' ',
  `Logo` varchar(110) DEFAULT ' ',
  `FinnhubIndustry` varchar(100) DEFAULT ' ',
  `Peer` varchar(200) DEFAULT ' ',
  `ProfileUpdateDate` int DEFAULT '19800101',
  `DayLineStartDate` int DEFAULT '29900101',
  `DayLineEndDate` int DEFAULT '19800101',
  `LastRTDataUpdateDate` int DEFAULT '19800101',
  `PeerUpdateDate` int DEFAULT '19800101',
  `InsiderTransactionUpdateDate` int DEFAULT '19800101',
  `LastEPSSurpriseUpdateDate` int DEFAULT '19800101',
  `IPOStatus` int DEFAULT '128',
  `TiingoPermaTicker` varchar(45) DEFAULT ' ',
  `IsActive` tinyint DEFAULT NULL,
  `IsADR` tinyint DEFAULT NULL,
  `TiingoIndustry` varchar(100) DEFAULT ' ',
  `TiingoSector` varchar(100) DEFAULT ' ',
  `SICCode` int DEFAULT '0',
  `SICIndustry` varchar(100) DEFAULT ' ',
  `SICSector` varchar(100) DEFAULT ' ',
  `CompanyWebSite` varchar(100) DEFAULT ' ',
  `SECFilingWebSite` varchar(150) DEFAULT ' ',
  `StatementUpdateDate` int DEFAULT '19800101',
  `DailyDataUpdateDate` int DEFAULT '19800101',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `ProfileUpdatedDate` (`ProfileUpdateDate`) /*!80000 INVISIBLE */,
  KEY `DayLineStartDate` (`DayLineStartDate`) /*!80000 INVISIBLE */,
  KEY `DayLineEndDate` (`DayLineEndDate`),
  KEY `LastRTUpdatedDate` (`LastRTDataUpdateDate`),
  KEY `Symbol` (`Symbol`,`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=304699 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tiingo_crypto_symbol`
--

DROP TABLE IF EXISTS `tiingo_crypto_symbol`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_crypto_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Ticker` varchar(45) DEFAULT '' COMMENT 'Ticker = BaseCurrency + QuoteCurrency',
  `BaseCurrency` varchar(20) DEFAULT '',
  `QuoteCurrency` varchar(20) DEFAULT '',
  `Name` varchar(45) DEFAULT '',
  `Description` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `ID` (`ID`) /*!80000 INVISIBLE */,
  KEY `BaseCurrency` (`BaseCurrency`) /*!80000 INVISIBLE */,
  KEY `QuoteCurrency` (`QuoteCurrency`) /*!80000 INVISIBLE */,
  KEY `Ticker` (`Ticker`)
) ENGINE=InnoDB AUTO_INCREMENT=14554 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='tiingo crypto代码';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tiingo_industry`
--

DROP TABLE IF EXISTS `tiingo_industry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Industry` varchar(100) DEFAULT NULL,
  `Sector` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Industry` (`Industry`,`Sector`)
) ENGINE=InnoDB AUTO_INCREMENT=2022 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tiingo_stock_fundanmental`
--

DROP TABLE IF EXISTS `tiingo_stock_fundanmental`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_stock_fundanmental` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `TiingoPermaTicker` varchar(45) DEFAULT ' ',
  `Ticker` varchar(45) DEFAULT ' ',
  `Name` varchar(200) DEFAULT ' ',
  `IsActive` tinyint DEFAULT NULL,
  `IsADR` tinyint DEFAULT NULL,
  `TiingoIndustry` varchar(100) DEFAULT ' ',
  `TiingoSector` varchar(100) DEFAULT ' ',
  `SICCode` int DEFAULT '0',
  `SICIndustry` varchar(100) DEFAULT ' ',
  `SICSector` varchar(100) DEFAULT ' ',
  `ReportingCurrency` varchar(45) DEFAULT ' ',
  `Location` varchar(100) DEFAULT ' ',
  `CompanyWebSite` varchar(100) DEFAULT ' ',
  `SECFilingWebSite` varchar(150) DEFAULT ' ',
  `StatementUpdateDate` int DEFAULT '19800101',
  `DailyDataUpdateDate` int DEFAULT '19800101',
  PRIMARY KEY (`ID`),
  KEY `Ticker` (`Ticker`)
) ENGINE=InnoDB AUTO_INCREMENT=57277 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `world_market_option`
--

DROP TABLE IF EXISTS `world_market_option`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `world_market_option` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `FinnhubToken` varchar(100) DEFAULT '',
  `TiingoToken` varchar(100) DEFAULT '',
  `QuandlToken` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-12-29  7:29:28
-- MySQL dump 10.13  Distrib 8.0.27, for Win64 (x86_64)
--
-- Host: localhost    Database: chinamarket_realtime_test
-- ------------------------------------------------------
-- Server version	8.0.27

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
-- Table structure for table `10rs1above55`
--

DROP TABLE IF EXISTS `10rs1above55`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `10rs1above55` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `10rs2above55`
--

DROP TABLE IF EXISTS `10rs2above55`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `10rs2above55` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `choiced_stock`
--

DROP TABLE IF EXISTS `choiced_stock`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choiced_stock` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `StockCode` (`Symbol`)
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
  `Date` int DEFAULT '0',
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
  KEY `time` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=1570467 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `currentweekline`
--

DROP TABLE IF EXISTS `currentweekline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `currentweekline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
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
  `OrdinaryBuyVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledSellAbove200000` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1398383 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='此表用于存储当前周的周线数据（所有的周数据皆位于此表中）';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dayline`
--

DROP TABLE IF EXISTS `dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `TIME` (`Date`,`UpDownRate`),
  KEY `STOCKCODE` (`Symbol`,`Date`),
  KEY `Day` (`Date`,`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=14256684 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daylineinfo`
--

DROP TABLE IF EXISTS `daylineinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `daylineinfo` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Symbol` varchar(10) DEFAULT ' ',
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
  `OrdinaryBuyVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledSellAbove200000` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `STOCKCODE` (`Symbol`,`Date`),
  KEY `TIME` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=15188861 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `option_calculatingrs`
--

DROP TABLE IF EXISTS `option_calculatingrs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `option_calculatingrs` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Index` int NOT NULL COMMENT '索引值（1-10），最多十个。',
  `Active` tinyint DEFAULT '0',
  `DayLengthFirst` int DEFAULT '0',
  `RSStrongFirst` decimal(10,3) DEFAULT '0.000',
  `StrongDayLengthFirst` int DEFAULT '0',
  `DayLengthSecond` int DEFAULT '0',
  `RSStrongSecond` decimal(10,3) DEFAULT '0.000',
  `StrongDayLengthSecond` int DEFAULT '0',
  `DayLengthThird` int DEFAULT '0',
  `RSStrongThird` decimal(10,3) DEFAULT '0.000',
  `StrongDayLengthThird` int DEFAULT '0',
  `DayLengthFourth` int DEFAULT '0',
  `RSStrongFourth` decimal(10,3) DEFAULT '0.000',
  `StrongDayLengthFourth` int DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `Index_UNIQUE` (`Index`),
  KEY `Index` (`Index`)
) ENGINE=InnoDB AUTO_INCREMENT=21301 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `option_china_stockmarket`
--

DROP TABLE IF EXISTS `option_china_stockmarket`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `option_china_stockmarket` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `RealTimeDataServer` int DEFAULT '0' COMMENT '实时数据服务器。默认为使用新浪事实数据服务器. 0：新浪实时数据服务器.1：网易实时数据服务器.2：腾讯实时数据服务器.\\n其他数值目前不支持。',
  `RealTimeDataInquiryTime` int DEFAULT '400' COMMENT '实时数据采集间隔。以毫秒为单位。',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `options`
--

DROP TABLE IF EXISTS `options`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `options` (
  `RelativeStrongStartDate` int NOT NULL DEFAULT '19900101',
  `RelativeStrongEndDate` int DEFAULT '19900101',
  `LastLogInDate` int DEFAULT '19900101',
  `LastLoginTime` int DEFAULT '0',
  `UpdatedDateFor10DaysRS1` int DEFAULT '19900101',
  `UpdatedDateFor10DaysRS2` int DEFAULT '19900101',
  `UpdatedDateFor10DaysRS` int DEFAULT '19900101',
  PRIMARY KEY (`RelativeStrongStartDate`),
  KEY `StartDay` (`RelativeStrongStartDate`)
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
  `Date` int DEFAULT '0',
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
  KEY `Day` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=437 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='Poten Daily Briefing for shipping Index.';
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
  `Symbol` varchar(12) DEFAULT '0',
  `StockName` varchar(12) DEFAULT '0',
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
  KEY `StockCode` (`Symbol`),
  KEY `Time` (`Time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_0`
--

DROP TABLE IF EXISTS `selected_rs_0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_0` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_1`
--

DROP TABLE IF EXISTS `selected_rs_1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_1` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_2`
--

DROP TABLE IF EXISTS `selected_rs_2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_2` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_3`
--

DROP TABLE IF EXISTS `selected_rs_3`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_3` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_4`
--

DROP TABLE IF EXISTS `selected_rs_4`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_4` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_5`
--

DROP TABLE IF EXISTS `selected_rs_5`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_5` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_6`
--

DROP TABLE IF EXISTS `selected_rs_6`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_6` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_7`
--

DROP TABLE IF EXISTS `selected_rs_7`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_7` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_8`
--

DROP TABLE IF EXISTS `selected_rs_8`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_8` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_9`
--

DROP TABLE IF EXISTS `selected_rs_9`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_9` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stock_code_section`
--

DROP TABLE IF EXISTS `stock_code_section`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stock_code_section` (
  `ID` int NOT NULL,
  `Active` tinyint DEFAULT NULL,
  `Market` int DEFAULT NULL,
  `IndexNumber` int DEFAULT NULL,
  `Comment` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `id_UNIQUE` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='市场证券代码段是否使用标志';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stockcode`
--

DROP TABLE IF EXISTS `stockcode`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stockcode` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Exchange` varchar(100) DEFAULT ' ',
  `Description` varchar(100) DEFAULT ' ',
  `Symbol` varchar(12) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
  `DayLineStartDate` int DEFAULT '29900101',
  `DayLineEndDate` int DEFAULT '19900101',
  `IPOStatus` int DEFAULT '128',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`,`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=5124 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `today`
--

DROP TABLE IF EXISTS `today`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `today` (
  `Id` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '0',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
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
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
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
  `OrdinaryBuyVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledSellAbove200000` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`Id`),
  UNIQUE KEY `Id_UNIQUE` (`Id`),
  KEY `time` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=4897 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weekline`
--

DROP TABLE IF EXISTS `weekline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `weekline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `TIME` (`Date`,`UpDownRate`),
  KEY `DAY` (`Date`,`Symbol`),
  KEY `Symbol` (`Symbol`,`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=3598077 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weeklineinfo`
--

DROP TABLE IF EXISTS `weeklineinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `weeklineinfo` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Symbol` varchar(10) DEFAULT ' ',
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
  `OrdinaryBuyVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledSellAbove200000` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `STOCKCODE` (`Symbol`,`Date`),
  KEY `TIME` (`Date`,`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=3598077 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-12-29  7:29:29
-- MySQL dump 10.13  Distrib 8.0.27, for Win64 (x86_64)
--
-- Host: localhost    Database: worldmarket_realtime_test
-- ------------------------------------------------------
-- Server version	8.0.27

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
-- Table structure for table `choiced_crypto`
--

DROP TABLE IF EXISTS `choiced_crypto`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choiced_crypto` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `choiced_forex`
--

DROP TABLE IF EXISTS `choiced_forex`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choiced_forex` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `choiced_stock`
--

DROP TABLE IF EXISTS `choiced_stock`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choiced_stock` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `country_list`
--

DROP TABLE IF EXISTS `country_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `country_list` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code2` varchar(45) DEFAULT ' ',
  `Code3` varchar(45) DEFAULT ' ',
  `CodeNo` varchar(45) DEFAULT ' ',
  `Country` varchar(100) DEFAULT ' ',
  `Currency` varchar(45) DEFAULT ' ',
  `CurrencyCode` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Code` (`Code2`) /*!80000 INVISIBLE */,
  KEY `CurrencyCode` (`CurrencyCode`)
) ENGINE=InnoDB AUTO_INCREMENT=501 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `crypto_dayline`
--

DROP TABLE IF EXISTS `crypto_dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `crypto_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19800101',
  `Exchange` varchar(50) DEFAULT ' ',
  `Symbol` varchar(50) DEFAULT ' ',
  `DisplaySymbol` varchar(45) DEFAULT '',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`,`Date`) /*!80000 INVISIBLE */,
  KEY `Date` (`Symbol`,`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=2338263 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `economic_calendar`
--

DROP TABLE IF EXISTS `economic_calendar`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `economic_calendar` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Time` bigint DEFAULT '0',
  `Country` varchar(45) DEFAULT ' ',
  `Event` varchar(100) DEFAULT ' ',
  `Impact` varchar(45) DEFAULT ' ',
  `Actual` decimal(10,3) DEFAULT '0.000',
  `estimate` decimal(10,3) DEFAULT '0.000',
  `Prev` decimal(10,3) DEFAULT '0.000',
  `Unit` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `eps_surprise`
--

DROP TABLE IF EXISTS `eps_surprise`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `eps_surprise` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT ' ',
  `Date` int DEFAULT '0',
  `Actual` double DEFAULT '0',
  `Estimate` double DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`),
  KEY `Date` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=103544 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_crypto_exchange`
--

DROP TABLE IF EXISTS `finnhub_crypto_exchange`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_crypto_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` varchar(45) DEFAULT '',
  `Name` varchar(100) DEFAULT '',
  `Mic` varchar(45) DEFAULT '',
  `TimeZone` varchar(45) DEFAULT '',
  `Hour` varchar(45) DEFAULT '',
  `CloseDate` varchar(45) DEFAULT '',
  `Country` varchar(45) DEFAULT '',
  `Source` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Exchange` (`Code`)
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_crypto_symbol`
--

DROP TABLE IF EXISTS `finnhub_crypto_symbol`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_crypto_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Description` varchar(100) DEFAULT ' ',
  `DisplaySymbol` varchar(100) DEFAULT ' ',
  `Exchange` varchar(100) DEFAULT ' ',
  `Symbol` varchar(100) DEFAULT ' ',
  `DayLineStartDate` int DEFAULT '29900101',
  `DayLineEndDate` int DEFAULT '19800101',
  `IPOStatus` int DEFAULT '128',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`) /*!80000 INVISIBLE */,
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=8367 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_forex_exchange`
--

DROP TABLE IF EXISTS `finnhub_forex_exchange`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_forex_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` varchar(45) DEFAULT ' ',
  `Name` varchar(100) DEFAULT '',
  `Mic` varchar(45) DEFAULT '',
  `TimeZone` varchar(45) DEFAULT '',
  `Hour` varchar(45) DEFAULT '',
  `CloseDate` varchar(45) DEFAULT '',
  `Country` varchar(45) DEFAULT '',
  `Source` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Exchange` (`Code`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_forex_symbol`
--

DROP TABLE IF EXISTS `finnhub_forex_symbol`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_forex_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Description` varchar(100) DEFAULT ' ',
  `DisplaySymbol` varchar(100) DEFAULT ' ',
  `Exchange` varchar(100) DEFAULT ' ',
  `Symbol` varchar(100) DEFAULT ' ',
  `DayLineStartDate` int DEFAULT '29900101',
  `DayLineEndDate` int DEFAULT '19800101',
  `IPOStatus` int DEFAULT '128',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=1425 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_profile`
--

DROP TABLE IF EXISTS `finnhub_profile`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_profile` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Address` varchar(100) DEFAULT ' ',
  `City` varchar(45) DEFAULT ' ',
  `Country` varchar(45) DEFAULT ' ',
  `Currency` varchar(45) DEFAULT ' ',
  `cusip` varchar(45) DEFAULT ' ',
  `sedol` varchar(45) DEFAULT ' ',
  `EmployeeTotal` int DEFAULT '0',
  `Exchange` varchar(100) DEFAULT ' ',
  `ggroup` varchar(45) DEFAULT ' ',
  `gind` varchar(45) DEFAULT ' ',
  `gsector` varchar(45) DEFAULT ' ',
  `gsubind` varchar(45) DEFAULT ' ',
  `IPODate` varchar(20) DEFAULT ' ',
  `isin` varchar(45) DEFAULT ' ',
  `MarketCapitalization` decimal(25,6) DEFAULT '0.000000',
  `naics` varchar(45) DEFAULT ' ',
  `naicsNationalIndustry` varchar(45) DEFAULT ' ',
  `naicsSector` varchar(45) DEFAULT ' ',
  `naicsSubsector` varchar(45) DEFAULT ' ',
  `Name` varchar(100) DEFAULT ' ',
  `Phone` varchar(100) DEFAULT ' ',
  `ShareOutstanding` decimal(20,6) DEFAULT '0.000000',
  `state` varchar(45) DEFAULT ' ',
  `Ticker` varchar(45) DEFAULT ' ',
  `WebURL` varchar(150) DEFAULT ' ',
  `Logo` varchar(110) DEFAULT ' ',
  `FinnhubIndustry` varchar(100) DEFAULT ' ',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_exchange`
--

DROP TABLE IF EXISTS `finnhub_stock_exchange`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` varchar(45) DEFAULT NULL,
  `Name` varchar(100) DEFAULT '',
  `Mic` varchar(45) DEFAULT '',
  `TimeZone` varchar(45) DEFAULT '',
  `Hour` varchar(45) DEFAULT '',
  `CloseDate` varchar(45) DEFAULT '',
  `Country` varchar(45) DEFAULT '',
  `Source` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=72 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_exchange_old`
--

DROP TABLE IF EXISTS `finnhub_stock_exchange_old`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_exchange_old` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `IsActive` tinyint DEFAULT '0',
  `Code` varchar(45) DEFAULT NULL,
  `Name` varchar(100) DEFAULT '',
  `Mic` varchar(45) DEFAULT '',
  `TimeZone` varchar(45) DEFAULT '',
  `Hour` varchar(45) DEFAULT '',
  `CloseDate` varchar(45) DEFAULT '',
  `Country` varchar(45) DEFAULT '',
  `Source` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=69 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_symbol`
--

DROP TABLE IF EXISTS `finnhub_stock_symbol`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Currency` varchar(45) DEFAULT ' ',
  `Description` varchar(200) DEFAULT ' ',
  `DisplaySymbol` varchar(45) DEFAULT ' ',
  `Figi` varchar(45) DEFAULT ' ',
  `Mic` varchar(45) DEFAULT ' ',
  `Symbol` varchar(45) DEFAULT ' ',
  `Type` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `forex_dayline`
--

DROP TABLE IF EXISTS `forex_dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `forex_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19800101',
  `Exchange` varchar(50) DEFAULT ' ',
  `Symbol` varchar(50) DEFAULT ' ',
  `DisplaySymbol` varchar(45) DEFAULT '',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Date` (`Date`),
  KEY `Symbol` (`Symbol`,`Date`) /*!80000 INVISIBLE */
) ENGINE=InnoDB AUTO_INCREMENT=1968032 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `insider_transaction`
--

DROP TABLE IF EXISTS `insider_transaction`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `insider_transaction` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `PersonName` varchar(200) DEFAULT NULL,
  `Share` decimal(15,0) DEFAULT NULL,
  `Change` decimal(15,0) DEFAULT NULL,
  `FilingDate` int DEFAULT NULL,
  `TransactionDate` int DEFAULT NULL,
  `TransactionCode` varchar(5) DEFAULT NULL,
  `TransactionPrice` double DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`) /*!80000 INVISIBLE */,
  KEY `TransactionDate` (`TransactionDate`) /*!80000 INVISIBLE */,
  KEY `PersonName` (`PersonName`)
) ENGINE=InnoDB AUTO_INCREMENT=283514 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `naics_industry`
--

DROP TABLE IF EXISTS `naics_industry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `naics_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Naics` varchar(45) DEFAULT ' ',
  `NationalIndustry` varchar(45) DEFAULT ' ',
  `Sector` varchar(45) DEFAULT ' ',
  `SubSector` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  KEY `NAICS` (`Naics`) /*!80000 INVISIBLE */,
  KEY `Industry` (`NationalIndustry`,`Sector`,`SubSector`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='NAICS行业标准';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sic_industry`
--

DROP TABLE IF EXISTS `sic_industry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `sic_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` int DEFAULT '0',
  `Industry` varchar(45) DEFAULT ' ',
  `Sector` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  KEY `SICCode` (`Code`) /*!80000 INVISIBLE */,
  KEY `Industry` (`Industry`,`Sector`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='SIC行业标准';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stock_dayline`
--

DROP TABLE IF EXISTS `stock_dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stock_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19800101',
  `Exchange` varchar(100) DEFAULT ' ',
  `Symbol` varchar(45) DEFAULT ' ',
  `DisplaySymbol` varchar(45) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Date` (`Date`,`Symbol`) /*!80000 INVISIBLE */,
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=13354924 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stock_profile`
--

DROP TABLE IF EXISTS `stock_profile`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stock_profile` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(20) DEFAULT ' ',
  `ExchangeCode` varchar(3) DEFAULT 'US',
  `Description` varchar(200) DEFAULT ' ',
  `DisplaySymbol` varchar(20) DEFAULT ' ',
  `Type` varchar(25) DEFAULT ' ',
  `Mic` varchar(20) DEFAULT ' ',
  `Figi` varchar(20) DEFAULT ' ',
  `Currency` varchar(20) DEFAULT ' ',
  `Address` varchar(100) DEFAULT ' ',
  `City` varchar(20) DEFAULT ' ',
  `Country` varchar(20) DEFAULT ' ',
  `cusip` varchar(20) DEFAULT ' ',
  `sedol` varchar(20) DEFAULT ' ',
  `EmployeeTotal` int DEFAULT '0',
  `ListedExchange` varchar(100) DEFAULT ' ' COMMENT '所上市的交易所名称',
  `ggroup` varchar(45) DEFAULT ' ',
  `gind` varchar(45) DEFAULT ' ',
  `gsector` varchar(45) DEFAULT ' ',
  `gsubind` varchar(45) DEFAULT ' ',
  `IPODate` varchar(20) DEFAULT ' ',
  `isin` varchar(45) DEFAULT ' ',
  `MarketCapitalization` decimal(25,6) DEFAULT '0.000000',
  `naics` varchar(45) DEFAULT ' ',
  `naicsNationalIndustry` varchar(100) DEFAULT ' ',
  `naicsSector` varchar(100) DEFAULT ' ',
  `naicsSubsector` varchar(100) DEFAULT ' ',
  `Name` varchar(200) DEFAULT ' ',
  `Phone` varchar(100) DEFAULT ' ',
  `ShareOutstanding` decimal(20,6) DEFAULT '0.000000',
  `state` varchar(45) DEFAULT ' ',
  `Ticker` varchar(45) DEFAULT ' ',
  `WebURL` varchar(150) DEFAULT ' ',
  `Logo` varchar(110) DEFAULT ' ',
  `FinnhubIndustry` varchar(100) DEFAULT ' ',
  `Peer` varchar(200) DEFAULT ' ',
  `ProfileUpdateDate` int DEFAULT '19800101',
  `DayLineStartDate` int DEFAULT '29900101',
  `DayLineEndDate` int DEFAULT '19800101',
  `LastRTDataUpdateDate` int DEFAULT '19800101',
  `PeerUpdateDate` int DEFAULT '19800101',
  `InsiderTransactionUpdateDate` int DEFAULT '19800101',
  `LastEPSSurpriseUpdateDate` int DEFAULT '19800101',
  `IPOStatus` int DEFAULT '128',
  `TiingoPermaTicker` varchar(45) DEFAULT ' ',
  `IsActive` tinyint DEFAULT NULL,
  `IsADR` tinyint DEFAULT NULL,
  `TiingoIndustry` varchar(100) DEFAULT ' ',
  `TiingoSector` varchar(100) DEFAULT ' ',
  `SICCode` int DEFAULT '0',
  `SICIndustry` varchar(100) DEFAULT ' ',
  `SICSector` varchar(100) DEFAULT ' ',
  `CompanyWebSite` varchar(100) DEFAULT ' ',
  `SECFilingWebSite` varchar(150) DEFAULT ' ',
  `StatementUpdateDate` int DEFAULT '19800101',
  `DailyDataUpdateDate` int DEFAULT '19800101',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `ProfileUpdatedDate` (`ProfileUpdateDate`),
  KEY `DayLineStartDate` (`DayLineStartDate`) /*!80000 INVISIBLE */,
  KEY `DayLineEndDate` (`DayLineEndDate`),
  KEY `LastRTUpdatedDate` (`LastRTDataUpdateDate`),
  KEY `Symbol` (`Symbol`,`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=283092 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='所有股票的基本情况。';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tiingo_crypto_symbol`
--

DROP TABLE IF EXISTS `tiingo_crypto_symbol`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_crypto_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Ticker` varchar(45) DEFAULT '' COMMENT 'Ticker = BaseCurrency + QuoteCurrency',
  `BaseCurrency` varchar(20) DEFAULT '',
  `QuoteCurrency` varchar(20) DEFAULT '',
  `Name` varchar(45) DEFAULT '',
  `Description` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `ID` (`ID`) /*!80000 INVISIBLE */,
  KEY `BaseCurrency` (`BaseCurrency`) /*!80000 INVISIBLE */,
  KEY `QuoteCurrency` (`QuoteCurrency`) /*!80000 INVISIBLE */,
  KEY `Ticker` (`Ticker`)
) ENGINE=InnoDB AUTO_INCREMENT=14554 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='tiingo crypto代码';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tiingo_industry`
--

DROP TABLE IF EXISTS `tiingo_industry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Industry` varchar(100) DEFAULT NULL,
  `Sector` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Industry` (`Industry`,`Sector`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tiingo_stock_fundanmental`
--

DROP TABLE IF EXISTS `tiingo_stock_fundanmental`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_stock_fundanmental` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `TiingoPermaTicker` varchar(45) DEFAULT ' ',
  `Ticker` varchar(45) DEFAULT ' ',
  `Name` varchar(200) DEFAULT ' ',
  `IsActive` tinyint DEFAULT NULL,
  `IsADR` tinyint DEFAULT NULL,
  `TiingoIndustry` varchar(100) DEFAULT ' ',
  `TiingoSector` varchar(100) DEFAULT ' ',
  `SICCode` int DEFAULT '0',
  `SICIndustry` varchar(100) DEFAULT ' ',
  `SICSector` varchar(100) DEFAULT ' ',
  `ReportingCurrency` varchar(45) DEFAULT ' ',
  `Location` varchar(100) DEFAULT ' ',
  `CompanyWebSite` varchar(100) DEFAULT ' ',
  `SECFilingWebSite` varchar(150) DEFAULT ' ',
  `StatementUpdateDate` int DEFAULT '19800101',
  `DailyDataUpdateDate` int DEFAULT '19800101',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=57214 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `world_market_option`
--

DROP TABLE IF EXISTS `world_market_option`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `world_market_option` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `FinnhubToken` varchar(100) DEFAULT '',
  `TiingoToken` varchar(100) DEFAULT '',
  `QuandlToken` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-12-29  7:29:29
-- MySQL dump 10.13  Distrib 8.0.27, for Win64 (x86_64)
--
-- Host: localhost    Database: chinamarket_test
-- ------------------------------------------------------
-- Server version	8.0.27

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
-- Table structure for table `10rs1above55`
--

DROP TABLE IF EXISTS `10rs1above55`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `10rs1above55` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `10rs2above55`
--

DROP TABLE IF EXISTS `10rs2above55`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `10rs2above55` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `choiced_stock`
--

DROP TABLE IF EXISTS `choiced_stock`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choiced_stock` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`) /*!80000 INVISIBLE */
) ENGINE=InnoDB AUTO_INCREMENT=3265 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `crweberindex`
--

DROP TABLE IF EXISTS `crweberindex`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `crweberindex` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '0',
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
  KEY `time` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=3110732 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `currentweekline`
--

DROP TABLE IF EXISTS `currentweekline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `currentweekline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
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
  `OrdinaryBuyVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledSellAbove200000` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=615110 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='此表用于存储当前周的周线数据（所有的周数据皆位于此表中）,与today表使用相同的数据。';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dayline`
--

DROP TABLE IF EXISTS `dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `TIME` (`Date`,`UpDownRate`),
  KEY `STOCKCODE` (`Symbol`,`Date`),
  KEY `Day` (`Date`,`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=13205638 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daylineinfo`
--

DROP TABLE IF EXISTS `daylineinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `daylineinfo` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Symbol` varchar(10) DEFAULT ' ',
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
  `OrdinaryBuyVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledSellAbove200000` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `STOCKCODE` (`Symbol`,`Date`),
  KEY `TIME` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=14402759 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `option_calculatingrs`
--

DROP TABLE IF EXISTS `option_calculatingrs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `option_calculatingrs` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Index` int NOT NULL COMMENT '索引值（1-10），最多十个。',
  `Active` tinyint DEFAULT '0',
  `DayLengthFirst` int DEFAULT '0',
  `RSStrongFirst` decimal(10,3) DEFAULT '0.000',
  `StrongDayLengthFirst` int DEFAULT '0',
  `DayLengthSecond` int DEFAULT '0',
  `RSStrongSecond` decimal(10,3) DEFAULT '0.000',
  `StrongDayLengthSecond` int DEFAULT '0',
  `DayLengthThird` int DEFAULT '0',
  `RSStrongThird` decimal(10,3) DEFAULT '0.000',
  `StrongDayLengthThird` int DEFAULT '0',
  `DayLengthFourth` int DEFAULT '0',
  `RSStrongFourth` decimal(10,3) DEFAULT '0.000',
  `StrongDayLengthFourth` int DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `Index_UNIQUE` (`Index`),
  KEY `Index` (`Index`)
) ENGINE=InnoDB AUTO_INCREMENT=30822 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `option_china_stockmarket`
--

DROP TABLE IF EXISTS `option_china_stockmarket`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `option_china_stockmarket` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `RealTimeDataServer` int DEFAULT '0' COMMENT '实时数据服务器。默认为使用新浪事实数据服务器. 0：新浪实时数据服务器.1：网易实时数据服务器.2：腾讯实时数据服务器.\\n其他数值目前不支持。',
  `RealTimeDataInquiryTime` int DEFAULT '400' COMMENT '实时数据采集间隔。以毫秒为单位。',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `options`
--

DROP TABLE IF EXISTS `options`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `options` (
  `RelativeStrongStartDate` int NOT NULL DEFAULT '19900101',
  `RelativeStrongEndDate` int DEFAULT '19900101',
  `LastLogInDate` int DEFAULT '19900101',
  `LastLoginTime` int DEFAULT '0',
  `UpdatedDateFor10DaysRS1` int DEFAULT '19900101',
  `UpdatedDateFor10DaysRS2` int DEFAULT '19900101',
  `UpdatedDateFor10DaysRS` int DEFAULT '19900101',
  PRIMARY KEY (`RelativeStrongStartDate`),
  KEY `StartDay` (`RelativeStrongStartDate`)
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
  `Date` int DEFAULT '0',
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
  KEY `Day` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=2109 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='Poten Daily Briefing for shipping Index.';
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
  `Symbol` varchar(12) DEFAULT '0',
  `StockName` varchar(12) DEFAULT '0',
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
  KEY `StockCode` (`Symbol`),
  KEY `Time` (`Time`)
) ENGINE=InnoDB AUTO_INCREMENT=3339 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_0`
--

DROP TABLE IF EXISTS `selected_rs_0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_0` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_1`
--

DROP TABLE IF EXISTS `selected_rs_1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_1` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_2`
--

DROP TABLE IF EXISTS `selected_rs_2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_2` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_3`
--

DROP TABLE IF EXISTS `selected_rs_3`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_3` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_4`
--

DROP TABLE IF EXISTS `selected_rs_4`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_4` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_5`
--

DROP TABLE IF EXISTS `selected_rs_5`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_5` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_6`
--

DROP TABLE IF EXISTS `selected_rs_6`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_6` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_7`
--

DROP TABLE IF EXISTS `selected_rs_7`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_7` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_8`
--

DROP TABLE IF EXISTS `selected_rs_8`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_8` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_9`
--

DROP TABLE IF EXISTS `selected_rs_9`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_9` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stock_code_section`
--

DROP TABLE IF EXISTS `stock_code_section`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stock_code_section` (
  `ID` int NOT NULL,
  `Active` tinyint DEFAULT NULL,
  `Market` int DEFAULT NULL,
  `IndexNumber` int DEFAULT NULL,
  `Comment` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `id_UNIQUE` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='市场证券代码段是否使用标志';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stockcode`
--

DROP TABLE IF EXISTS `stockcode`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stockcode` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Description` varchar(100) DEFAULT ' ',
  `Exchange` varchar(100) DEFAULT ' ',
  `Symbol` varchar(12) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
  `DayLineStartDate` int DEFAULT '29900101',
  `DayLineEndDate` int DEFAULT '19900101',
  `IPOStatus` int DEFAULT '128',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`,`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=8368 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `today`
--

DROP TABLE IF EXISTS `today`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `today` (
  `Id` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '0',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
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
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
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
  `OrdinaryBuyVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledSellAbove200000` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`Id`),
  UNIQUE KEY `Id_UNIQUE` (`Id`),
  KEY `time` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=6095 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='与CurrentWeekLine表使用相同的数据。';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weekline`
--

DROP TABLE IF EXISTS `weekline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `weekline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `TIME` (`Date`,`UpDownRate`),
  KEY `DAY` (`Date`,`Symbol`),
  KEY `Symbol` (`Symbol`,`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=2849770 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weeklineinfo`
--

DROP TABLE IF EXISTS `weeklineinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `weeklineinfo` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Symbol` varchar(10) DEFAULT ' ',
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
  `OrdinaryBuyVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledSellAbove200000` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `STOCKCODE` (`Symbol`,`Date`),
  KEY `TIME` (`Date`,`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=2846447 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-12-29  7:29:29
-- MySQL dump 10.13  Distrib 8.0.27, for Win64 (x86_64)
--
-- Host: localhost    Database: chinamarket
-- ------------------------------------------------------
-- Server version	8.0.27

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
-- Table structure for table `10rs1above55`
--

DROP TABLE IF EXISTS `10rs1above55`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `10rs1above55` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `10rs2above55`
--

DROP TABLE IF EXISTS `10rs2above55`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `10rs2above55` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `choiced_stock`
--

DROP TABLE IF EXISTS `choiced_stock`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choiced_stock` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `StockCode` (`Symbol`)
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
  `Date` int DEFAULT '0',
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
  KEY `time` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=1716446 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `currentweekline`
--

DROP TABLE IF EXISTS `currentweekline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `currentweekline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
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
  `OrdinaryBuyVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledSellAbove200000` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1599684 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='此表用于存储当前周的周线数据（所有的周数据皆位于此表中）';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dayline`
--

DROP TABLE IF EXISTS `dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Time` (`Date`,`UpDownRate`),
  KEY `STOCKCODE` (`Symbol`,`Date`),
  KEY `Day` (`Date`,`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=14460818 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daylineinfo`
--

DROP TABLE IF EXISTS `daylineinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `daylineinfo` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Symbol` varchar(10) DEFAULT ' ',
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
  `OrdinaryBuyVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledSellAbove200000` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `STOCKCODE` (`Symbol`,`Date`),
  KEY `TIME` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=15390084 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `option_calculatingrs`
--

DROP TABLE IF EXISTS `option_calculatingrs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `option_calculatingrs` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Index` int NOT NULL COMMENT '索引值（1-10），最多十个。',
  `Active` tinyint DEFAULT '0',
  `DayLengthFirst` int DEFAULT '0',
  `RSStrongFirst` decimal(10,3) DEFAULT '0.000',
  `StrongDayLengthFirst` int DEFAULT '0',
  `DayLengthSecond` int DEFAULT '0',
  `RSStrongSecond` decimal(10,3) DEFAULT '0.000',
  `StrongDayLengthSecond` int DEFAULT '0',
  `DayLengthThird` int DEFAULT '0',
  `RSStrongThird` decimal(10,3) DEFAULT '0.000',
  `StrongDayLengthThird` int DEFAULT '0',
  `DayLengthFourth` int DEFAULT '0',
  `RSStrongFourth` decimal(10,3) DEFAULT '0.000',
  `StrongDayLengthFourth` int DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `Index_UNIQUE` (`Index`),
  KEY `Index` (`Index`)
) ENGINE=InnoDB AUTO_INCREMENT=23531 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `option_china_stockmarket`
--

DROP TABLE IF EXISTS `option_china_stockmarket`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `option_china_stockmarket` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `RealTimeDataServer` int DEFAULT '0' COMMENT '实时数据服务器。默认为使用新浪事实数据服务器. 0：新浪实时数据服务器.1：网易实时数据服务器.2：腾讯实时数据服务器.\\n其他数值目前不支持。',
  `RealTimeDataInquiryTime` int DEFAULT '400' COMMENT '实时数据采集间隔。以毫秒为单位。',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `options`
--

DROP TABLE IF EXISTS `options`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `options` (
  `RelativeStrongStartDate` int NOT NULL DEFAULT '19900101',
  `RelativeStrongEndDate` int DEFAULT '19900101',
  `LastLoginDate` int DEFAULT '19900101',
  `LastLoginTime` int DEFAULT '0',
  `UpdatedDateFor10DaysRS1` int DEFAULT '19900101',
  `UpdatedDateFor10DaysRS2` int DEFAULT '19900101',
  `UpdatedDateFor10DaysRS` int DEFAULT '19900101',
  PRIMARY KEY (`RelativeStrongStartDate`),
  KEY `StartDay` (`RelativeStrongStartDate`)
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
  `Date` int DEFAULT '0',
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
  KEY `Day` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=437 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='Poten Daily Briefing for shipping Index.';
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
  `Symbol` varchar(12) DEFAULT '0',
  `StockName` varchar(12) DEFAULT '0',
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
  KEY `StockCode` (`Symbol`),
  KEY `Time` (`Time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_0`
--

DROP TABLE IF EXISTS `selected_rs_0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_0` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_1`
--

DROP TABLE IF EXISTS `selected_rs_1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_1` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_2`
--

DROP TABLE IF EXISTS `selected_rs_2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_2` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_3`
--

DROP TABLE IF EXISTS `selected_rs_3`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_3` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_4`
--

DROP TABLE IF EXISTS `selected_rs_4`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_4` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_5`
--

DROP TABLE IF EXISTS `selected_rs_5`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_5` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_6`
--

DROP TABLE IF EXISTS `selected_rs_6`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_6` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_7`
--

DROP TABLE IF EXISTS `selected_rs_7`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_7` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_8`
--

DROP TABLE IF EXISTS `selected_rs_8`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_8` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selected_rs_9`
--

DROP TABLE IF EXISTS `selected_rs_9`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selected_rs_9` (
  `Symbol` varchar(10) NOT NULL,
  PRIMARY KEY (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stock_code_section`
--

DROP TABLE IF EXISTS `stock_code_section`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stock_code_section` (
  `ID` int NOT NULL,
  `Active` tinyint DEFAULT NULL,
  `Market` int DEFAULT NULL,
  `IndexNumber` int DEFAULT NULL,
  `Comment` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `id_UNIQUE` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='市场证券代码段是否使用标志';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stockcode`
--

DROP TABLE IF EXISTS `stockcode`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stockcode` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Exchange` varchar(100) DEFAULT ' ',
  `Description` varchar(100) DEFAULT ' ',
  `Symbol` varchar(12) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
  `DayLineStartDate` int DEFAULT '29900101',
  `DayLineEndDate` int DEFAULT '19900101',
  `IPOStatus` int DEFAULT '128',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`,`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=5160 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `today`
--

DROP TABLE IF EXISTS `today`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `today` (
  `Id` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '0',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
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
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
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
  `OrdinaryBuyVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledSellAbove200000` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`Id`),
  UNIQUE KEY `Id_UNIQUE` (`Id`),
  KEY `time` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=4926 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weekline`
--

DROP TABLE IF EXISTS `weekline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `weekline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `TIME` (`Date`,`UpDownRate`),
  KEY `DAY` (`Date`,`Symbol`),
  KEY `Symbol` (`Symbol`,`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=3799378 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weeklineinfo`
--

DROP TABLE IF EXISTS `weeklineinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `weeklineinfo` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Symbol` varchar(10) DEFAULT ' ',
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
  `OrdinaryBuyVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinaryBuyNumberAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellVolumeAbove200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow5000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow10000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow20000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow50000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow100000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberBelow200000` decimal(15,0) DEFAULT '0',
  `OrdinarySellNumberAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledBuyBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledBuyAbove200000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow5000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow10000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow20000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow50000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow100000` decimal(15,0) DEFAULT '0',
  `CanceledSellBelow200000` decimal(15,0) DEFAULT '0',
  `CanceledSellAbove200000` decimal(15,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `STOCKCODE` (`Symbol`,`Date`),
  KEY `TIME` (`Date`,`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=3799378 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-12-29  7:29:29
-- MySQL dump 10.13  Distrib 8.0.27, for Win64 (x86_64)
--
-- Host: localhost    Database: worldmarket
-- ------------------------------------------------------
-- Server version	8.0.27

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
-- Table structure for table `choiced_crypto`
--

DROP TABLE IF EXISTS `choiced_crypto`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choiced_crypto` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `choiced_forex`
--

DROP TABLE IF EXISTS `choiced_forex`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choiced_forex` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `choiced_stock`
--

DROP TABLE IF EXISTS `choiced_stock`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choiced_stock` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `country_list`
--

DROP TABLE IF EXISTS `country_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `country_list` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code2` varchar(45) DEFAULT ' ',
  `Code3` varchar(45) DEFAULT ' ',
  `CodeNo` varchar(45) DEFAULT ' ',
  `Country` varchar(100) DEFAULT ' ',
  `Currency` varchar(45) DEFAULT ' ',
  `CurrencyCode` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Code` (`Code2`) /*!80000 INVISIBLE */,
  KEY `CurrencyCode` (`CurrencyCode`)
) ENGINE=InnoDB AUTO_INCREMENT=499 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `crypto_dayline`
--

DROP TABLE IF EXISTS `crypto_dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `crypto_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19800101',
  `Exchange` varchar(50) DEFAULT ' ',
  `Symbol` varchar(50) DEFAULT ' ',
  `DisplaySymbol` varchar(45) DEFAULT '',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`,`Date`) /*!80000 INVISIBLE */,
  KEY `Date` (`Symbol`,`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=2338263 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `economic_calendar`
--

DROP TABLE IF EXISTS `economic_calendar`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `economic_calendar` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Time` bigint DEFAULT '0',
  `Country` varchar(45) DEFAULT ' ',
  `Event` varchar(100) DEFAULT ' ',
  `Impact` varchar(45) DEFAULT ' ',
  `Actual` decimal(10,3) DEFAULT '0.000',
  `estimate` decimal(10,3) DEFAULT '0.000',
  `Prev` decimal(10,3) DEFAULT '0.000',
  `Unit` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `eps_surprise`
--

DROP TABLE IF EXISTS `eps_surprise`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `eps_surprise` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT ' ',
  `Date` int DEFAULT '0',
  `Actual` double DEFAULT '0',
  `Estimate` double DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`),
  KEY `Date` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=103544 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_crypto_exchange`
--

DROP TABLE IF EXISTS `finnhub_crypto_exchange`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_crypto_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` varchar(45) DEFAULT '',
  `Name` varchar(100) DEFAULT '',
  `Mic` varchar(45) DEFAULT '',
  `TimeZone` varchar(45) DEFAULT '',
  `Hour` varchar(45) DEFAULT '',
  `CloseDate` varchar(45) DEFAULT '',
  `Country` varchar(45) DEFAULT '',
  `Source` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Exchange` (`Code`)
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_crypto_symbol`
--

DROP TABLE IF EXISTS `finnhub_crypto_symbol`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_crypto_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Description` varchar(100) DEFAULT ' ',
  `DisplaySymbol` varchar(100) DEFAULT ' ',
  `Exchange` varchar(100) DEFAULT ' ',
  `Symbol` varchar(100) DEFAULT ' ',
  `DayLineStartDate` int DEFAULT '29900101',
  `DayLineEndDate` int DEFAULT '19800101',
  `IPOStatus` int DEFAULT '128',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`) /*!80000 INVISIBLE */,
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=8367 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_forex_exchange`
--

DROP TABLE IF EXISTS `finnhub_forex_exchange`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_forex_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` varchar(45) DEFAULT ' ',
  `Name` varchar(100) DEFAULT '',
  `Mic` varchar(45) DEFAULT '',
  `TimeZone` varchar(45) DEFAULT '',
  `Hour` varchar(45) DEFAULT '',
  `CloseDate` varchar(45) DEFAULT '',
  `Country` varchar(45) DEFAULT '',
  `Source` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Exchange` (`Code`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_forex_symbol`
--

DROP TABLE IF EXISTS `finnhub_forex_symbol`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_forex_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Description` varchar(100) DEFAULT ' ',
  `DisplaySymbol` varchar(100) DEFAULT ' ',
  `Exchange` varchar(100) DEFAULT ' ',
  `Symbol` varchar(100) DEFAULT ' ',
  `DayLineStartDate` int DEFAULT '29900101',
  `DayLineEndDate` int DEFAULT '19800101',
  `IPOStatus` int DEFAULT '128',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=1425 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_profile`
--

DROP TABLE IF EXISTS `finnhub_profile`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_profile` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Address` varchar(100) DEFAULT ' ',
  `City` varchar(45) DEFAULT ' ',
  `Country` varchar(45) DEFAULT ' ',
  `Currency` varchar(45) DEFAULT ' ',
  `cusip` varchar(45) DEFAULT ' ',
  `sedol` varchar(45) DEFAULT ' ',
  `EmployeeTotal` int DEFAULT '0',
  `Exchange` varchar(100) DEFAULT ' ',
  `ggroup` varchar(45) DEFAULT ' ',
  `gind` varchar(45) DEFAULT ' ',
  `gsector` varchar(45) DEFAULT ' ',
  `gsubind` varchar(45) DEFAULT ' ',
  `IPODate` varchar(20) DEFAULT ' ',
  `isin` varchar(45) DEFAULT ' ',
  `MarketCapitalization` decimal(25,6) DEFAULT '0.000000',
  `naics` varchar(45) DEFAULT ' ',
  `naicsNationalIndustry` varchar(45) DEFAULT ' ',
  `naicsSector` varchar(45) DEFAULT ' ',
  `naicsSubsector` varchar(45) DEFAULT ' ',
  `Name` varchar(100) DEFAULT ' ',
  `Phone` varchar(100) DEFAULT ' ',
  `ShareOutstanding` decimal(20,6) DEFAULT '0.000000',
  `state` varchar(45) DEFAULT ' ',
  `Ticker` varchar(45) DEFAULT ' ',
  `WebURL` varchar(150) DEFAULT ' ',
  `Logo` varchar(110) DEFAULT ' ',
  `FinnhubIndustry` varchar(100) DEFAULT ' ',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_exchange`
--

DROP TABLE IF EXISTS `finnhub_stock_exchange`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` varchar(45) DEFAULT NULL,
  `Name` varchar(100) DEFAULT '',
  `Mic` varchar(45) DEFAULT '',
  `TimeZone` varchar(45) DEFAULT '',
  `Hour` varchar(45) DEFAULT '',
  `CloseDate` varchar(45) DEFAULT '',
  `Country` varchar(45) DEFAULT '',
  `Source` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=71 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_exchange_old`
--

DROP TABLE IF EXISTS `finnhub_stock_exchange_old`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_exchange_old` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `IsActive` tinyint DEFAULT '0',
  `Code` varchar(45) DEFAULT NULL,
  `Name` varchar(100) DEFAULT '',
  `Mic` varchar(45) DEFAULT '',
  `TimeZone` varchar(45) DEFAULT '',
  `Hour` varchar(45) DEFAULT '',
  `CloseDate` varchar(45) DEFAULT '',
  `Country` varchar(45) DEFAULT '',
  `Source` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=69 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_symbol`
--

DROP TABLE IF EXISTS `finnhub_stock_symbol`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Currency` varchar(45) DEFAULT ' ',
  `Description` varchar(200) DEFAULT ' ',
  `DisplaySymbol` varchar(45) DEFAULT ' ',
  `Figi` varchar(45) DEFAULT ' ',
  `Mic` varchar(45) DEFAULT ' ',
  `Symbol` varchar(45) DEFAULT ' ',
  `Type` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `forex_dayline`
--

DROP TABLE IF EXISTS `forex_dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `forex_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19800101',
  `Exchange` varchar(50) DEFAULT ' ',
  `Symbol` varchar(50) DEFAULT ' ',
  `DisplaySymbol` varchar(45) DEFAULT '',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Date` (`Date`),
  KEY `Symbol` (`Symbol`,`Date`) /*!80000 INVISIBLE */
) ENGINE=InnoDB AUTO_INCREMENT=1968032 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `insider_transaction`
--

DROP TABLE IF EXISTS `insider_transaction`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `insider_transaction` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `PersonName` varchar(200) DEFAULT NULL,
  `Share` decimal(15,0) DEFAULT NULL,
  `Change` decimal(15,0) DEFAULT NULL,
  `FilingDate` int DEFAULT NULL,
  `TransactionDate` int DEFAULT NULL,
  `TransactionCode` varchar(5) DEFAULT NULL,
  `TransactionPrice` double DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`) /*!80000 INVISIBLE */,
  KEY `TransactionDate` (`TransactionDate`) /*!80000 INVISIBLE */,
  KEY `PersonName` (`PersonName`)
) ENGINE=InnoDB AUTO_INCREMENT=283514 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `naics_industry`
--

DROP TABLE IF EXISTS `naics_industry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `naics_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Naics` varchar(45) DEFAULT ' ',
  `NationalIndustry` varchar(45) DEFAULT ' ',
  `Sector` varchar(45) DEFAULT ' ',
  `SubSector` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  KEY `NAICS` (`Naics`) /*!80000 INVISIBLE */,
  KEY `Industry` (`NationalIndustry`,`Sector`,`SubSector`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='NAICS行业标准';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sic_industry`
--

DROP TABLE IF EXISTS `sic_industry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `sic_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` int DEFAULT '0',
  `Industry` varchar(45) DEFAULT ' ',
  `Sector` varchar(45) DEFAULT ' ',
  PRIMARY KEY (`ID`),
  KEY `SICCode` (`Code`) /*!80000 INVISIBLE */,
  KEY `Industry` (`Industry`,`Sector`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='SIC行业标准';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stock_dayline`
--

DROP TABLE IF EXISTS `stock_dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stock_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19800101',
  `Exchange` varchar(100) DEFAULT ' ',
  `Symbol` varchar(45) DEFAULT ' ',
  `DisplaySymbol` varchar(45) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `RelativeStrong` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongIndex` decimal(10,3) DEFAULT '0.000',
  `RelativeStrongBackup` decimal(10,3) DEFAULT '0.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Date` (`Date`,`Symbol`) /*!80000 INVISIBLE */,
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=13354928 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stock_profile`
--

DROP TABLE IF EXISTS `stock_profile`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stock_profile` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(20) DEFAULT ' ',
  `ExchangeCode` varchar(3) DEFAULT 'US',
  `Description` varchar(200) DEFAULT ' ',
  `DisplaySymbol` varchar(20) DEFAULT ' ',
  `Type` varchar(25) DEFAULT ' ',
  `Mic` varchar(20) DEFAULT ' ',
  `Figi` varchar(20) DEFAULT ' ',
  `Currency` varchar(20) DEFAULT ' ',
  `Address` varchar(100) DEFAULT ' ',
  `City` varchar(20) DEFAULT ' ',
  `Country` varchar(20) DEFAULT ' ',
  `cusip` varchar(20) DEFAULT ' ',
  `sedol` varchar(20) DEFAULT ' ',
  `EmployeeTotal` int DEFAULT '0',
  `ListedExchange` varchar(100) DEFAULT ' ' COMMENT '所上市的交易所名称',
  `ggroup` varchar(45) DEFAULT ' ',
  `gind` varchar(45) DEFAULT ' ',
  `gsector` varchar(45) DEFAULT ' ',
  `gsubind` varchar(45) DEFAULT ' ',
  `IPODate` varchar(20) DEFAULT ' ',
  `isin` varchar(45) DEFAULT ' ',
  `MarketCapitalization` decimal(25,6) DEFAULT '0.000000',
  `naics` varchar(45) DEFAULT ' ',
  `naicsNationalIndustry` varchar(100) DEFAULT ' ',
  `naicsSector` varchar(100) DEFAULT ' ',
  `naicsSubsector` varchar(100) DEFAULT ' ',
  `Name` varchar(200) DEFAULT ' ',
  `Phone` varchar(100) DEFAULT ' ',
  `ShareOutstanding` decimal(20,6) DEFAULT '0.000000',
  `state` varchar(45) DEFAULT ' ',
  `Ticker` varchar(45) DEFAULT ' ',
  `WebURL` varchar(150) DEFAULT ' ',
  `Logo` varchar(110) DEFAULT ' ',
  `FinnhubIndustry` varchar(100) DEFAULT ' ',
  `Peer` varchar(200) DEFAULT ' ',
  `ProfileUpdateDate` int DEFAULT '19800101',
  `DayLineStartDate` int DEFAULT '29900101',
  `DayLineEndDate` int DEFAULT '19800101',
  `LastRTDataUpdateDate` int DEFAULT '19800101',
  `PeerUpdateDate` int DEFAULT '19800101',
  `InsiderTransactionUpdateDate` int DEFAULT '19800101',
  `LastEPSSurpriseUpdateDate` int DEFAULT '19800101',
  `IPOStatus` int DEFAULT '128',
  `TiingoPermaTicker` varchar(45) DEFAULT ' ',
  `IsActive` tinyint DEFAULT NULL,
  `IsADR` tinyint DEFAULT NULL,
  `TiingoIndustry` varchar(100) DEFAULT ' ',
  `TiingoSector` varchar(100) DEFAULT ' ',
  `SICCode` int DEFAULT '0',
  `SICIndustry` varchar(100) DEFAULT ' ',
  `SICSector` varchar(100) DEFAULT ' ',
  `CompanyWebSite` varchar(100) DEFAULT ' ',
  `SECFilingWebSite` varchar(150) DEFAULT ' ',
  `StatementUpdateDate` int DEFAULT '19800101',
  `DailyDataUpdateDate` int DEFAULT '19800101',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `ProfileUpdatedDate` (`ProfileUpdateDate`),
  KEY `DayLineStartDate` (`DayLineStartDate`) /*!80000 INVISIBLE */,
  KEY `DayLineEndDate` (`DayLineEndDate`),
  KEY `LastRTUpdatedDate` (`LastRTDataUpdateDate`),
  KEY `Symbol` (`Symbol`,`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=283717 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='所有股票的基本情况。';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tiingo_crypto_symbol`
--

DROP TABLE IF EXISTS `tiingo_crypto_symbol`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_crypto_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Ticker` varchar(45) DEFAULT '' COMMENT 'Ticker = BaseCurrency + QuoteCurrency',
  `BaseCurrency` varchar(20) DEFAULT '',
  `QuoteCurrency` varchar(20) DEFAULT '',
  `Name` varchar(45) DEFAULT '',
  `Description` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `ID` (`ID`) /*!80000 INVISIBLE */,
  KEY `BaseCurrency` (`BaseCurrency`) /*!80000 INVISIBLE */,
  KEY `QuoteCurrency` (`QuoteCurrency`) /*!80000 INVISIBLE */,
  KEY `Ticker` (`Ticker`)
) ENGINE=InnoDB AUTO_INCREMENT=14554 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='tiingo crypto代码';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tiingo_industry`
--

DROP TABLE IF EXISTS `tiingo_industry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Industry` varchar(100) DEFAULT NULL,
  `Sector` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Industry` (`Industry`,`Sector`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tiingo_stock_fundanmental`
--

DROP TABLE IF EXISTS `tiingo_stock_fundanmental`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_stock_fundanmental` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `TiingoPermaTicker` varchar(45) DEFAULT ' ',
  `Ticker` varchar(45) DEFAULT ' ',
  `Name` varchar(200) DEFAULT ' ',
  `IsActive` tinyint DEFAULT NULL,
  `IsADR` tinyint DEFAULT NULL,
  `TiingoIndustry` varchar(100) DEFAULT ' ',
  `TiingoSector` varchar(100) DEFAULT ' ',
  `SICCode` int DEFAULT '0',
  `SICIndustry` varchar(100) DEFAULT ' ',
  `SICSector` varchar(100) DEFAULT ' ',
  `ReportingCurrency` varchar(45) DEFAULT ' ',
  `Location` varchar(100) DEFAULT ' ',
  `CompanyWebSite` varchar(100) DEFAULT ' ',
  `SECFilingWebSite` varchar(150) DEFAULT ' ',
  `StatementUpdateDate` int DEFAULT '19800101',
  `DailyDataUpdateDate` int DEFAULT '19800101',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=57213 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `world_market_option`
--

DROP TABLE IF EXISTS `world_market_option`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `world_market_option` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `FinnhubToken` varchar(100) DEFAULT '',
  `TiingoToken` varchar(100) DEFAULT '',
  `QuandlToken` varchar(100) DEFAULT '',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-12-29  7:29:29
