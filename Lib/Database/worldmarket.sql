-- MySQL dump 10.13  Distrib 8.0.38, for Win64 (x86_64)
--
-- Host: localhost    Database: worldmarket
-- ------------------------------------------------------
-- Server version	8.4.2

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
-- Table structure for table `choice_crypto`
--

DROP TABLE IF EXISTS `choice_crypto`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choice_crypto` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `choice_forex`
--

DROP TABLE IF EXISTS `choice_forex`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choice_forex` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `choice_stock`
--

DROP TABLE IF EXISTS `choice_stock`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `choice_stock` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=66 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_company_exective`
--

DROP TABLE IF EXISTS `finnhub_company_exective`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_company_exective` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `CompanySymbol` varchar(45) DEFAULT NULL,
  `Age` int DEFAULT NULL,
  `Compensation` int DEFAULT NULL,
  `Currency` varchar(20) DEFAULT NULL,
  `Name` varchar(45) DEFAULT NULL,
  `Position` varchar(100) DEFAULT NULL,
  `Sex` varchar(20) DEFAULT NULL,
  `Since` int DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`CompanySymbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_company_news`
--

DROP TABLE IF EXISTS `finnhub_company_news`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_company_news` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `Category` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `DateTime` decimal(30,0) DEFAULT NULL,
  `Headline` varchar(1000) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `NewsID` int DEFAULT NULL,
  `Image` varchar(1000) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `RelatedSymbol` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `Source` varchar(500) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `Summary` varchar(10000) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `URL` varchar(1000) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `DateTime` (`DateTime`),
  KEY `Symbol` (`Symbol`,`DateTime`)
) ENGINE=InnoDB AUTO_INCREMENT=12248995 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_country_list`
--

DROP TABLE IF EXISTS `finnhub_country_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_country_list` (
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
) ENGINE=InnoDB AUTO_INCREMENT=535 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_crypto_dayline`
--

DROP TABLE IF EXISTS `finnhub_crypto_dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_crypto_dayline` (
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
) ENGINE=InnoDB AUTO_INCREMENT=2979531 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_crypto_exchange`
--

DROP TABLE IF EXISTS `finnhub_crypto_exchange`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_crypto_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `code` varchar(45) DEFAULT '',
  `name` varchar(100) DEFAULT '',
  `mic` varchar(45) DEFAULT '',
  `timezone` varchar(45) DEFAULT '',
  `pre_market` text,
  `post_market` text,
  `hour` varchar(45) DEFAULT '',
  `close_date` varchar(45) DEFAULT '',
  `country` varchar(45) DEFAULT '',
  `country_name` text,
  `source` varchar(100) DEFAULT '',
  `MyUnknownColumn` text,
  PRIMARY KEY (`ID`),
  KEY `Exchange` (`code`)
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
  `IPOStatus` int DEFAULT '128',
  `UpdateDate` varchar(10000) DEFAULT '',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`) /*!80000 INVISIBLE */,
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=402551 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_economic_calendar`
--

DROP TABLE IF EXISTS `finnhub_economic_calendar`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_economic_calendar` (
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
-- Table structure for table `finnhub_forex_dayline`
--

DROP TABLE IF EXISTS `finnhub_forex_dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_forex_dayline` (
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
-- Table structure for table `finnhub_forex_exchange`
--

DROP TABLE IF EXISTS `finnhub_forex_exchange`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_forex_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `code` varchar(45) DEFAULT ' ',
  `name` varchar(100) DEFAULT '',
  `mic` varchar(45) DEFAULT '',
  `timezone` varchar(45) DEFAULT '',
  `pre_market` varchar(45) DEFAULT NULL,
  `hour` varchar(45) DEFAULT '',
  `post_market` text,
  `close_date` varchar(45) DEFAULT '',
  `country` varchar(45) DEFAULT '',
  `country_name` text,
  `source` varchar(100) DEFAULT '',
  `MyUnknownColumn` text,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Exchange` (`code`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
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
  `IPOStatus` int DEFAULT '128',
  `UpdateDate` varchar(10000) DEFAULT '',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=1436 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_insider_sentiment`
--

DROP TABLE IF EXISTS `finnhub_insider_sentiment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_insider_sentiment` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Date` int DEFAULT NULL,
  `Change` int DEFAULT NULL,
  `mspr` double DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=266223 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_insider_transaction`
--

DROP TABLE IF EXISTS `finnhub_insider_transaction`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_insider_transaction` (
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
  KEY `PersonName` (`PersonName`),
  KEY `TransactionCode` (`TransactionCode`),
  KEY `Symbol` (`Symbol`,`TransactionDate`) /*!80000 INVISIBLE */,
  KEY `TransactionDate` (`TransactionDate`,`Symbol`) /*!80000 INVISIBLE */
) ENGINE=InnoDB AUTO_INCREMENT=3485357 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_basic_financials_annual`
--

DROP TABLE IF EXISTS `finnhub_stock_basic_financials_annual`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_basic_financials_annual` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Type` varchar(100) DEFAULT NULL COMMENT '"cashRatio"、 "currentRatio"。。。。',
  `Date` int DEFAULT NULL,
  `Value` double DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`,`Date`,`Type`)
) ENGINE=InnoDB AUTO_INCREMENT=7709334 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_basic_financials_metric`
--

DROP TABLE IF EXISTS `finnhub_stock_basic_financials_metric`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_basic_financials_metric` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `10DayAverageTradingVolume` double DEFAULT NULL,
  `13WeekPriceReturnDaily` double DEFAULT NULL,
  `26WeekPriceReturnDaily` double DEFAULT NULL,
  `3MonthAverageTradingVolume` double DEFAULT NULL,
  `52WeekHigh` double DEFAULT NULL,
  `52WeekHighDate` int DEFAULT NULL,
  `52WeekLow` double DEFAULT NULL,
  `52WeekLowDate` int DEFAULT NULL,
  `52WeekPriceReturnDaily` double DEFAULT NULL,
  `5DayPriceReturnDaily` double DEFAULT NULL,
  `assetTurnoverAnnual` double DEFAULT NULL,
  `assetTurnoverTTM` double DEFAULT NULL,
  `Beta` double DEFAULT NULL,
  `bookValuePerShareAnnual` double DEFAULT NULL,
  `bookValuePerShareQuarterly` double DEFAULT NULL,
  `bookValueShareGrowth5Y` double DEFAULT NULL,
  `capitalSpendingGrowth5Y` double DEFAULT NULL,
  `cashFlowPerShareAnnual` double DEFAULT NULL,
  `cashFlowPerShareTTM` double DEFAULT NULL,
  `cashPerSharePerShareAnnual` double DEFAULT NULL,
  `cashPerSharePerShareQuarterly` double DEFAULT NULL,
  `currentDividendYieldTTM` double DEFAULT NULL,
  `currentEV_freeCashFlowAnnual` double DEFAULT NULL,
  `currentEV_freeCashFlowTTM` double DEFAULT NULL,
  `currentRatioAnnual` double DEFAULT NULL,
  `currentRatioQuarterly` double DEFAULT NULL,
  `dividendGrowthRate5Y` double DEFAULT NULL,
  `dividendPerShare5Y` double DEFAULT NULL,
  `dividendPerShareAnnual` double DEFAULT NULL,
  `dividendYield5Y` double DEFAULT NULL,
  `dividendYieldIndicatedAnnual` double DEFAULT NULL,
  `dividendsPerShareTTM` double DEFAULT NULL,
  `ebitdPerShareTTM` double DEFAULT NULL,
  `ebitdaCagr5Y` double DEFAULT NULL,
  `ebitdaInterimCagr5Y` double DEFAULT NULL,
  `epsBasicExclExtraItemsAnnual` double DEFAULT NULL,
  `epsBasicExclExtraitemsTTM` double DEFAULT NULL,
  `epsExclExtraItemsAnnual` double DEFAULT NULL,
  `epsExclExtraItemsTTM` double DEFAULT NULL,
  `epsGrowth3Y` double DEFAULT NULL,
  `epsGrowth5Y` double DEFAULT NULL,
  `epsGrowthQuarterlyYoy` double DEFAULT NULL,
  `epsGrowthTTMYoy` double DEFAULT NULL,
  `epsInclExtraItemsAnnual` double DEFAULT NULL,
  `epsInclExtraItemsTTM` double DEFAULT NULL,
  `epsNormalizedAnnual` double DEFAULT NULL,
  `focfCagr5Y` double DEFAULT NULL,
  `freeCashFlowAnnual` double DEFAULT NULL,
  `freeCashFlowPerShareTTM` double DEFAULT NULL,
  `freeCashFlowTTM` double DEFAULT NULL,
  `freeOperatingCashFlow_revenue5Y` double DEFAULT NULL,
  `freeOperatingCashFlow_revenueTTM` double DEFAULT NULL,
  `grossMargin5Y` double DEFAULT NULL,
  `grossMarginAnnual` double DEFAULT NULL,
  `grossMarginTTM` double DEFAULT NULL,
  `inventoryTurnoverAnnual` double DEFAULT NULL,
  `inventoryTurnoverTTM` double DEFAULT NULL,
  `longTermDebt_equityAnnual` double DEFAULT NULL,
  `longTermDebt_equityQuarterly` double DEFAULT NULL,
  `marketCapitalization` double DEFAULT NULL,
  `monthToDatePriceReturnDaily` double DEFAULT NULL,
  `netDebtAnnual` double DEFAULT NULL,
  `netDebtInterim` double DEFAULT NULL,
  `netIncomeEmployeeAnnual` double DEFAULT NULL,
  `netIncomeEmployeeTTM` double DEFAULT NULL,
  `netInterestCoverageAnnual` double DEFAULT NULL,
  `netInterestCoverageTTM` double DEFAULT NULL,
  `netMarginGrowth5Y` double DEFAULT NULL,
  `netProfitMargin5Y` double DEFAULT NULL,
  `netProfitMarginAnnual` double DEFAULT NULL,
  `netProfitMarginTTM` double DEFAULT NULL,
  `operatingMargin5Y` double DEFAULT NULL,
  `operatingMarginAnnual` double DEFAULT NULL,
  `operatingMarginTTM` double DEFAULT NULL,
  `payoutRatioAnnual` double DEFAULT NULL,
  `payoutRatioTTM` double DEFAULT NULL,
  `pbAnnual` double DEFAULT NULL,
  `pbQuarterly` double DEFAULT NULL,
  `pcfShareTTM` double DEFAULT NULL,
  `peBasicExclExtraTTM` double DEFAULT NULL,
  `peExclExtraAnnual` double DEFAULT NULL,
  `peExclExtraHighTTM` double DEFAULT NULL,
  `peExclExtraTTM` double DEFAULT NULL,
  `peExclLowTTM` double DEFAULT NULL,
  `peInclExtraTTM` double DEFAULT NULL,
  `peNormalizedAnnual` double DEFAULT NULL,
  `pfcfShareTTM` double DEFAULT NULL,
  `pfcfShareAnnual` double DEFAULT NULL,
  `pretaxMargin5Y` double DEFAULT NULL,
  `pretaxMarginAnnual` double DEFAULT NULL,
  `pretaxMarginTTM` double DEFAULT NULL,
  `priceRelativeToSP50013Week` double DEFAULT NULL,
  `priceRelativeToSP50026Week` double DEFAULT NULL,
  `priceRelativeToSP5004Week` double DEFAULT NULL,
  `priceRelativeToSP50052Week` double DEFAULT NULL,
  `priceRelativeToSP500Ytd` double DEFAULT NULL,
  `psAnnual` double DEFAULT NULL,
  `psTTM` double DEFAULT NULL,
  `ptbvAnnual` double DEFAULT NULL,
  `ptbvQuarterly` double DEFAULT NULL,
  `quickRatioAnnual` double DEFAULT NULL,
  `quickRatioQuarterly` double DEFAULT NULL,
  `receivablesTurnoverAnnual` double DEFAULT NULL,
  `receivablesTurnoverTTM` double DEFAULT NULL,
  `revenueEmployeeAnnual` double DEFAULT NULL,
  `revenueEmployeeTTM` double DEFAULT NULL,
  `revenueGrowth3Y` double DEFAULT NULL,
  `revenueGrowth5Y` double DEFAULT NULL,
  `revenueGrowthQuarterlyYoy` double DEFAULT NULL,
  `revenueGrowthTTMYoy` double DEFAULT NULL,
  `revenuePerShareAnnual` double DEFAULT NULL,
  `revenuePerShareTTM` double DEFAULT NULL,
  `revenueShareGrowth5Y` double DEFAULT NULL,
  `roaRfy` double DEFAULT NULL,
  `roaa5Y` double DEFAULT NULL,
  `roae5Y` double DEFAULT NULL,
  `roaeTTM` double DEFAULT NULL,
  `roeRfy` double DEFAULT NULL,
  `roeTTM` double DEFAULT NULL,
  `roi5Y` double DEFAULT NULL,
  `roiAnnual` double DEFAULT NULL,
  `roiTTM` double DEFAULT NULL,
  `tangibleBookValuePerShareAnnual` double DEFAULT NULL,
  `tangibleBookValuePerShareQuarterly` double DEFAULT NULL,
  `tbvCagr5Y` double DEFAULT NULL,
  `totalDebt_totalEquityAnnual` double DEFAULT NULL,
  `totalDebt_totalEquityQuarterly` double DEFAULT NULL,
  `totalDebtCagr5Y` double DEFAULT NULL,
  `yearToDatePriceReturnDaily` double DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=43823 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_basic_financials_quarterly`
--

DROP TABLE IF EXISTS `finnhub_stock_basic_financials_quarterly`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_basic_financials_quarterly` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Type` varchar(100) DEFAULT NULL COMMENT '"cashRatio"、 "currentRatio"。。。。',
  `Date` int DEFAULT NULL,
  `Value` double DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`,`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=22515593 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_dayline`
--

DROP TABLE IF EXISTS `finnhub_stock_dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_dayline` (
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
  KEY `Date` (`Date`,`Symbol`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=21945682 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_estimates_eps_surprise`
--

DROP TABLE IF EXISTS `finnhub_stock_estimates_eps_surprise`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_estimates_eps_surprise` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT ' ',
  `Date` int DEFAULT '0',
  `Actual` double DEFAULT '0',
  `Estimate` double DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`),
  KEY `Date` (`Date`)
) ENGINE=InnoDB AUTO_INCREMENT=170467 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_exchange`
--

DROP TABLE IF EXISTS `finnhub_stock_exchange`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `code` text,
  `name` text,
  `mic` text,
  `timezone` text,
  `pre_market` text,
  `hour` text,
  `post_market` text,
  `close_date` text,
  `country` text,
  `country_name` text,
  `source` text,
  `MyUnknownColumn` text,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=82 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_profile`
--

DROP TABLE IF EXISTS `finnhub_stock_profile`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_profile` (
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
  `Peer` varchar(2000) DEFAULT ' ',
  `IPOStatus` int DEFAULT '128',
  `TiingoPermaTicker` varchar(45) DEFAULT ' ',
  `IsActive` tinyint DEFAULT '1',
  `IsADR` tinyint DEFAULT '0',
  `TiingoIndustry` varchar(100) DEFAULT ' ',
  `TiingoSector` varchar(100) DEFAULT ' ',
  `SICCode` int DEFAULT '0',
  `SICIndustry` varchar(100) DEFAULT ' ',
  `SICSector` varchar(100) DEFAULT ' ',
  `CompanyWebSite` varchar(100) DEFAULT ' ',
  `SECFilingWebSite` varchar(150) DEFAULT ' ',
  `UpdateDate` varchar(10000) DEFAULT '{}' COMMENT '这个用于存储json制式的各更新日期。这样就能使用固定结构的stock_profile表了。',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=57179 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='所有股票的基本情况。';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `finnhub_stock_sec_filings`
--

DROP TABLE IF EXISTS `finnhub_stock_sec_filings`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `finnhub_stock_sec_filings` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `symbol` varchar(45) DEFAULT NULL,
  `accessNumber` varchar(100) DEFAULT NULL,
  `cik` int DEFAULT NULL,
  `form` varchar(100) DEFAULT NULL,
  `acceptedDate` int DEFAULT NULL,
  `filedDate` int DEFAULT NULL,
  `filingURL` varchar(200) DEFAULT NULL,
  `reportURL` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `symbol` (`symbol`,`accessNumber`) /*!80000 INVISIBLE */
) ENGINE=InnoDB AUTO_INCREMENT=164725 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
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
-- Table structure for table `tiingo_company_financial_state`
--

DROP TABLE IF EXISTS `tiingo_company_financial_state`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_company_financial_state` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `symbol` varchar(45) DEFAULT NULL,
  `Exchange` varchar(45) DEFAULT NULL,
  `yearQuarter` int DEFAULT NULL,
  `accoci` decimal(30,0) DEFAULT NULL,
  `acctPay` decimal(30,0) DEFAULT NULL,
  `acctRec` decimal(30,0) DEFAULT NULL,
  `assetsCurrent` decimal(30,0) DEFAULT NULL,
  `assetsNonCurrent` varchar(45) DEFAULT NULL,
  `assetTurnover` varchar(45) DEFAULT NULL,
  `bookVal` varchar(45) DEFAULT NULL,
  `businessAcqDisposals` varchar(45) DEFAULT NULL,
  `bvps` varchar(45) DEFAULT NULL,
  `capex` varchar(45) DEFAULT NULL,
  `cashAndEq` varchar(45) DEFAULT NULL,
  `consolidatedIncome` varchar(45) DEFAULT NULL,
  `costRev` varchar(45) DEFAULT NULL,
  `currentRatio` varchar(45) DEFAULT NULL,
  `debt` varchar(45) DEFAULT NULL,
  `debtCurrent` varchar(45) DEFAULT NULL,
  `debtEquity` varchar(45) DEFAULT NULL,
  `debtNonCurrent` varchar(45) DEFAULT NULL,
  `deferredRev` varchar(45) DEFAULT NULL,
  `depamor` varchar(45) DEFAULT NULL,
  `deposits` varchar(45) DEFAULT NULL,
  `ebit` varchar(45) DEFAULT NULL,
  `ebitda` varchar(45) DEFAULT NULL,
  `ebt` varchar(45) DEFAULT NULL,
  `enterpriseVal` varchar(45) DEFAULT NULL,
  `eps` varchar(45) DEFAULT NULL,
  `epsDil` varchar(45) DEFAULT NULL,
  `epsQoQ` varchar(45) DEFAULT NULL,
  `equity` varchar(45) DEFAULT NULL,
  `freeCashFlow` varchar(45) DEFAULT NULL,
  `fxRate` varchar(45) DEFAULT NULL,
  `grossMargin` varchar(45) DEFAULT NULL,
  `grossProfit` varchar(45) DEFAULT NULL,
  `intangibles` varchar(45) DEFAULT NULL,
  `intexp` varchar(45) DEFAULT NULL,
  `inventory` varchar(45) DEFAULT NULL,
  `investments` varchar(45) DEFAULT NULL,
  `investmentsAcqDisposals` varchar(45) DEFAULT NULL,
  `investmentsCurrent` varchar(45) DEFAULT NULL,
  `investmentsNonCurrent` varchar(45) DEFAULT NULL,
  `issrepayDebt` varchar(45) DEFAULT NULL,
  `issrepayEquity` varchar(45) DEFAULT NULL,
  `liabilitiesCurrent` varchar(45) DEFAULT NULL,
  `liabilitiesNonCurrent` varchar(45) DEFAULT NULL,
  `longTermDebtEquity` varchar(45) DEFAULT NULL,
  `marketCap` varchar(45) DEFAULT NULL,
  `ncf` varchar(45) DEFAULT NULL,
  `ncff` varchar(45) DEFAULT NULL,
  `ncfi` varchar(45) DEFAULT NULL,
  `ncfo` varchar(45) DEFAULT NULL,
  `ncfx` varchar(45) DEFAULT NULL,
  `netinc` varchar(45) DEFAULT NULL,
  `netIncComStock` varchar(45) DEFAULT NULL,
  `netIncDiscOps` varchar(45) DEFAULT NULL,
  `netMargin` varchar(45) DEFAULT NULL,
  `nonControllingInterests` varchar(45) DEFAULT NULL,
  `opex` varchar(45) DEFAULT NULL,
  `opinc` varchar(45) DEFAULT NULL,
  `opMargin` varchar(45) DEFAULT NULL,
  `payDiv` varchar(45) DEFAULT NULL,
  `pbRatio` varchar(45) DEFAULT NULL,
  `peRatio` varchar(45) DEFAULT NULL,
  `piotroskiFScore` varchar(45) DEFAULT NULL,
  `ppeq` varchar(45) DEFAULT NULL,
  `prefDVDs` varchar(45) DEFAULT NULL,
  `profitMargin` varchar(45) DEFAULT NULL,
  `retainedEarnings` varchar(45) DEFAULT NULL,
  `revenue` varchar(45) DEFAULT NULL,
  `revenueQoQ` varchar(45) DEFAULT NULL,
  `rnd` varchar(45) DEFAULT NULL,
  `roa` varchar(45) DEFAULT NULL,
  `roe` varchar(45) DEFAULT NULL,
  `rps` varchar(45) DEFAULT NULL,
  `sbcomp` varchar(45) DEFAULT NULL,
  `sga` varchar(45) DEFAULT NULL,
  `shareFactor` varchar(45) DEFAULT NULL,
  `sharesBasic` varchar(45) DEFAULT NULL,
  `shareswa` varchar(45) DEFAULT NULL,
  `shareswaDil` varchar(45) DEFAULT NULL,
  `taxAssets` varchar(45) DEFAULT NULL,
  `taxExp` varchar(45) DEFAULT NULL,
  `taxLiabilities` varchar(45) DEFAULT NULL,
  `totalAssets` varchar(45) DEFAULT NULL,
  `totalLiabilities` varchar(45) DEFAULT NULL,
  `trailingPEG1Y` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `symbol` (`symbol`,`yearQuarter`),
  KEY `year` (`yearQuarter`,`symbol`),
  KEY `ID` (`ID`,`symbol`,`yearQuarter`),
  KEY `exchange` (`Exchange`,`symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=428 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
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
  `Name` varchar(100) DEFAULT '',
  `Description` varchar(100) DEFAULT '',
  `UpdateDate` varchar(10000) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `ID` (`ID`) /*!80000 INVISIBLE */,
  KEY `BaseCurrency` (`BaseCurrency`) /*!80000 INVISIBLE */,
  KEY `QuoteCurrency` (`QuoteCurrency`) /*!80000 INVISIBLE */,
  KEY `Ticker` (`Ticker`)
) ENGINE=InnoDB AUTO_INCREMENT=238563 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='tiingo crypto代码';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tiingo_fundamental_definitions`
--

DROP TABLE IF EXISTS `tiingo_fundamental_definitions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_fundamental_definitions` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `dataCode` varchar(45) DEFAULT '',
  `name` varchar(100) DEFAULT '',
  `description` mediumtext,
  `statementType` varchar(45) DEFAULT '',
  `units` varchar(45) DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `dataCode` (`dataCode`,`statementType`),
  KEY `name` (`name`,`statementType`),
  KEY `statementType` (`statementType`,`dataCode`)
) ENGINE=InnoDB AUTO_INCREMENT=229 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
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
-- Table structure for table `tiingo_market_news`
--

DROP TABLE IF EXISTS `tiingo_market_news`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_market_news` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `ids` int DEFAULT NULL,
  `source` varchar(100) DEFAULT NULL,
  `description` varchar(500) DEFAULT NULL,
  `url` varchar(100) DEFAULT NULL,
  `tags` varchar(400) DEFAULT NULL,
  `tickers` varchar(200) DEFAULT NULL,
  `title` varchar(100) DEFAULT NULL,
  `crawlDate` varchar(100) DEFAULT NULL,
  `publishedDate` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `ID` (`ID`) /*!80000 INVISIBLE */,
  KEY `ticker` (`tickers`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tiingo_stock_dayline`
--

DROP TABLE IF EXISTS `tiingo_stock_dayline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_stock_dayline` (
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
  KEY `Date` (`Date`,`Symbol`),
  KEY `Symbol` (`Symbol`)
) ENGINE=InnoDB AUTO_INCREMENT=63159 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tiingo_stock_fundamental`
--

DROP TABLE IF EXISTS `tiingo_stock_fundamental`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tiingo_stock_fundamental` (
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
  `UpdateDate` varchar(10000) DEFAULT '{}',
  PRIMARY KEY (`ID`),
  KEY `ticker` (`Ticker`) /*!80000 INVISIBLE */,
  KEY `sector` (`SICSector`)
) ENGINE=InnoDB AUTO_INCREMENT=59631 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
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
  `FinnhubMaxPerHour` int DEFAULT '3000',
  `TiingoToken` varchar(100) DEFAULT '',
  `TiingoMaxPerHour` int DEFAULT '450',
  `QuandlToken` varchar(100) DEFAULT '',
  `QuandlMaxPerHour` int DEFAULT '100',
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

-- Dump completed on 2024-10-14 18:39:40
