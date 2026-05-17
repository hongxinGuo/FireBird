-- MySQL dump 10.13  Distrib 8.0.19, for Win64 (x86_64)
--
-- Host: localhost    Database: stock_market
-- ------------------------------------------------------
-- Server version	8.4.8

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `tiingo_stock_fundamental`
--

DROP TABLE IF EXISTS `tiingo_stock_fundamental`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_stock_fundamental` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `TiingoPermaTicker` varchar(45) DEFAULT '',
  `Ticker` varchar(45) DEFAULT '',
  `Name` varchar(200) DEFAULT '',
  `IsActive` int DEFAULT NULL,
  `IsADR` int DEFAULT NULL,
  `TiingoIndustry` varchar(100) DEFAULT '',
  `TiingoSector` varchar(100) DEFAULT '',
  `SICCode` int DEFAULT '0',
  `SICIndustry` varchar(100) DEFAULT '',
  `SICSector` varchar(100) DEFAULT '',
  `ReportingCurrency` varchar(45) DEFAULT '',
  `Location` varchar(100) DEFAULT '',
  `CompanyWebSite` varchar(100) DEFAULT '',
  `SECFilingWebSite` varchar(150) DEFAULT '',
  `IPOStatus` int DEFAULT '128',
  `UpdateDate` varchar(10000) DEFAULT '{}',
  PRIMARY KEY (`ID`),
  KEY `ticker` (`Ticker`) /*!80000 INVISIBLE */,
  KEY `SICCode` (`SICCode`,`Ticker`)
) ENGINE=InnoDB AUTO_INCREMENT=8666 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-05-17  4:52:50
