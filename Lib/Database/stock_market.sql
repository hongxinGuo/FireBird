-- MySQL dump 10.13  Distrib 8.0.19, for Win64 (x86_64)
--
-- Host: localhost    Database: stock_market
-- ------------------------------------------------------
-- Server version	9.7.1

CREATE TABLE `china_choice_stock` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(10) NOT NULL DEFAULT '‘ ’'
);

CREATE TABLE `china_market_options` (
  `LastLoginDate` int NOT NULL DEFAULT '19900101',
  `LastLoginTime` int NOT NULL DEFAULT '0'
);

CREATE TABLE `china_stock_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int NOT NULL DEFAULT '19900101',
  `Exchange` varchar(45) NOT NULL DEFAULT ' ',
  `Symbol` varchar(20) NOT NULL DEFAULT ' ',
  `LastClose` decimal(20,3) NOT NULL DEFAULT '0.000',
  `Open` decimal(20,3) NOT NULL DEFAULT '0.000',
  `High` decimal(20,3) NOT NULL DEFAULT '0.000',
  `Low` decimal(20,3) NOT NULL DEFAULT '0.000',
  `Close` decimal(20,3) NOT NULL DEFAULT '0.000',
  `Volume` decimal(28,0) NOT NULL DEFAULT '0',
  `Amount` decimal(30,0) NOT NULL DEFAULT '0',
  `Dividend` decimal(10,3) NOT NULL DEFAULT '0.000',
  `SplitFactor` decimal(10,3) NOT NULL DEFAULT '1.000',
  `UpAndDown` decimal(10,3) NOT NULL DEFAULT '0.000',
  `UpDownRate` decimal(10,3) NOT NULL DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) NOT NULL DEFAULT '0.000',
  `TotalValue` decimal(20,0) NOT NULL DEFAULT '0',
  `CurrentValue` decimal(20,0) NOT NULL DEFAULT '0'
);

CREATE TABLE `china_stock_profile` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Exchange` varchar(100) NOT NULL DEFAULT ' ',
  `Description` varchar(100) NOT NULL DEFAULT ' ',
  `Symbol` varchar(12) NOT NULL DEFAULT ' ',
  `DisplaySymbol` varchar(12) NOT NULL DEFAULT ' ',
  `UpdateDate` varchar(5000) NOT NULL DEFAULT '{}'
);

CREATE TABLE `china_stock_symbol_section` (
  `ID` int NOT NULL,
  `Active` tinyint NOT NULL DEFAULT '0',
  `Market` int NOT NULL DEFAULT '0',
  `IndexNumber` int NOT NULL DEFAULT '0',
  `Comment` varchar(200) NOT NULL DEFAULT ' '
);

CREATE TABLE `finnhub_company_exective` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `CompanySymbol` varchar(45)  NOT NULL DEFAULT ' ',
  `Age` int NOT NULL DEFAULT '0',
  `Compensation` int NOT NULL DEFAULT '0',
  `Currency` varchar(20)  NOT NULL DEFAULT ' ',
  `Name` varchar(45)  NOT NULL DEFAULT ' ',
  `Position` varchar(100)  NOT NULL DEFAULT ' ',
  `Sex` varchar(20)  NOT NULL DEFAULT ' ',
  `Since` int NOT NULL DEFAULT '0'
);

CREATE TABLE `finnhub_company_news` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45)  NOT NULL DEFAULT ' ',
  `Category` varchar(100)  NOT NULL DEFAULT ' ',
  `DateTime` int NOT NULL DEFAULT '0',
  `Headline` varchar(800)  NOT NULL DEFAULT ' ',
  `NewsID` int NOT NULL DEFAULT '0',
  `Image` varchar(500)  NOT NULL DEFAULT ' ',
  `RelatedSymbol` varchar(45)  NOT NULL DEFAULT ' ',
  `Source` varchar(500)  NOT NULL DEFAULT ' ',
  `Summary` varchar(3000)  NOT NULL DEFAULT ' ',
  `URL` varchar(500)  NOT NULL DEFAULT ' '
);

CREATE TABLE `finnhub_country_list` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code2` varchar(45)  NOT NULL DEFAULT ' ',
  `Code3` varchar(45)  NOT NULL DEFAULT ' ',
  `CodeNo` varchar(45)  NOT NULL DEFAULT ' ',
  `Country` varchar(100)  NOT NULL DEFAULT ' ',
  `Currency` varchar(45)  NOT NULL DEFAULT ' ',
  `CurrencyCode` varchar(45)  NOT NULL DEFAULT ' '
);

CREATE TABLE `finnhub_crypto_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int NOT NULL DEFAULT '19800101',
  `Exchange` varchar(50)  NOT NULL DEFAULT ' ',
  `Symbol` varchar(50)  NOT NULL DEFAULT ' ',
  `LastClose` decimal(10,3) NOT NULL DEFAULT '0.000',
  `Open` decimal(10,3) NOT NULL DEFAULT '0.000',
  `High` decimal(10,3) NOT NULL DEFAULT '0.000',
  `Low` decimal(10,3) NOT NULL DEFAULT '0.000',
  `Close` decimal(10,3) NOT NULL DEFAULT '0.000',
  `Volume` decimal(18,0) NOT NULL DEFAULT '0',
  `Amount` decimal(20,0) NOT NULL DEFAULT '0',
  `Dividend` decimal(10,3) NOT NULL DEFAULT '0.000',
  `SplitFactor` decimal(10,5) NOT NULL DEFAULT '1.00000',
  `UpAndDown` decimal(10,3) NOT NULL DEFAULT '0.000',
  `UpDownRate` decimal(10,3) NOT NULL DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) NOT NULL DEFAULT '0.000',
  `TotalValue` decimal(20,0) NOT NULL DEFAULT '0',
  `CurrentValue` decimal(20,0) NOT NULL DEFAULT '0'
);

CREATE TABLE `finnhub_crypto_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `code` varchar(45)  NOT NULL DEFAULT ' ',
  `name` varchar(100)  NOT NULL DEFAULT ' ',
  `mic` varchar(45)  NOT NULL DEFAULT ' ',
  `timezone` varchar(45)  NOT NULL DEFAULT ' ',
  `pre_market` varchar(45)  NOT NULL DEFAULT ' ',
  `post_market` varchar(45)  NOT NULL DEFAULT ' ',
  `hour` varchar(45)  NOT NULL DEFAULT ' ',
  `close_date` varchar(45)  NOT NULL DEFAULT ' ',
  `country` varchar(45)  NOT NULL DEFAULT ' ',
  `country_name` varchar(100)  NOT NULL DEFAULT ' ',
  `source` varchar(100)  NOT NULL DEFAULT ' '
);

CREATE TABLE `finnhub_crypto_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Description` varchar(100)  NOT NULL DEFAULT ' ',
  `DisplaySymbol` varchar(100)  NOT NULL DEFAULT ' ',
  `Exchange` varchar(100)  NOT NULL DEFAULT ' ',
  `Symbol` varchar(100)  NOT NULL DEFAULT ' ',
  `UpdateDate` varchar(5000)  NOT NULL DEFAULT '{}'
);

CREATE TABLE `finnhub_economic_calendar` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Time` varchar(45)  NOT NULL DEFAULT '0',
  `Country` varchar(45)  NOT NULL DEFAULT ' ',
  `Event` varchar(100)  NOT NULL DEFAULT ' ',
  `Impact` varchar(45)  NOT NULL DEFAULT ' ',
  `Actual` decimal(10,3) NOT NULL DEFAULT '0.000',
  `Estimate` decimal(10,3) NOT NULL DEFAULT '0.000',
  `Prev` decimal(10,3) NOT NULL DEFAULT '0.000',
  `Unit` varchar(45)  NOT NULL DEFAULT ' '
);

CREATE TABLE `finnhub_forex_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19800101',
  `Exchange` varchar(50) DEFAULT ' ',
  `Symbol` varchar(50) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `Dividend` decimal(10,3) DEFAULT '0.000',
  `SplitFactor` decimal(10,3) DEFAULT '1.000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0'
);

CREATE TABLE `finnhub_forex_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `code` varchar(45)  NOT NULL DEFAULT ' ',
  `name` varchar(100)  NOT NULL DEFAULT ' ',
  `mic` varchar(45)  NOT NULL DEFAULT ' ',
  `timezone` varchar(45)  NOT NULL DEFAULT ' ',
  `pre_market` varchar(45)  NOT NULL DEFAULT ' ',
  `hour` varchar(45)  NOT NULL DEFAULT ' ',
  `post_market` varchar(45)  NOT NULL DEFAULT ' ',
  `close_date` varchar(45)  NOT NULL DEFAULT ' ',
  `country` varchar(45)  NOT NULL DEFAULT ' ',
  `country_name` varchar(100)  NOT NULL DEFAULT ' ',
  `source` varchar(100)  NOT NULL DEFAULT ' '
);

CREATE TABLE `finnhub_forex_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Description` varchar(100)  NOT NULL DEFAULT ' ',
  `DisplaySymbol` varchar(100)  NOT NULL DEFAULT ' ',
  `Exchange` varchar(100)  NOT NULL DEFAULT ' ',
  `Symbol` varchar(100)  NOT NULL DEFAULT ' ',
  `UpdateDate` varchar(5000)  NOT NULL DEFAULT '{}'
);

CREATE TABLE `finnhub_insider_sentiment` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Date` int DEFAULT NULL,
  `Change` int DEFAULT NULL,
  `mspr` double DEFAULT NULL
);

CREATE TABLE `finnhub_insider_transaction` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `PersonName` varchar(200) DEFAULT NULL,
  `Share` decimal(20,0) DEFAULT NULL,
  `ShareChange` decimal(20,0) DEFAULT NULL,
  `FilingDate` int DEFAULT NULL,
  `TransactionDate` int DEFAULT NULL,
  `TransactionCode` varchar(5) DEFAULT NULL,
  `TransactionPrice` double DEFAULT NULL
);

CREATE TABLE `finnhub_stock_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int NOT NULL DEFAULT '19800101',
  `Exchange` varchar(50)  NOT NULL DEFAULT ' ',
  `Symbol` varchar(50)  NOT NULL DEFAULT ' ',
  `LastClose` decimal(10,3) NOT NULL DEFAULT '0.000',
  `Open` decimal(10,3) NOT NULL DEFAULT '0.000',
  `High` decimal(10,3) NOT NULL DEFAULT '0.000',
  `Low` decimal(10,3) NOT NULL DEFAULT '0.000',
  `Close` decimal(10,3) NOT NULL DEFAULT '0.000',
  `Volume` decimal(18,0) NOT NULL DEFAULT '0',
  `Amount` decimal(20,0) NOT NULL DEFAULT '0',
  `Dividend` decimal(10,3) NOT NULL DEFAULT '0.000',
  `SplitFactor` decimal(10,3) NOT NULL DEFAULT '1.000',
  `UpAndDown` decimal(10,3) NOT NULL DEFAULT '0.000',
  `UpDownRate` decimal(10,3) NOT NULL DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) NOT NULL DEFAULT '0.000',
  `TotalValue` decimal(20,0) NOT NULL DEFAULT '0',
  `CurrentValue` decimal(20,0) NOT NULL DEFAULT '0'
);

CREATE TABLE `finnhub_stock_estimates_eps_surprise` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT ' ',
  `Date` int DEFAULT '0',
  `Actual` double DEFAULT '0',
  `Estimate` double DEFAULT '0'
);

CREATE TABLE `finnhub_stock_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `code` varchar(45)  NOT NULL DEFAULT ' ',
  `name` varchar(100)  NOT NULL DEFAULT ' ',
  `mic` varchar(45)  NOT NULL DEFAULT ' ',
  `timezone` varchar(45)  NOT NULL DEFAULT ' ',
  `pre_market` varchar(45)  NOT NULL DEFAULT ' ',
  `hour` varchar(45)  NOT NULL DEFAULT ' ',
  `post_market` varchar(45)  NOT NULL DEFAULT ' ',
  `close_date` varchar(45)  NOT NULL DEFAULT ' ',
  `country` varchar(45)  NOT NULL DEFAULT ' ',
  `country_name` varchar(100)  NOT NULL DEFAULT ' ',
  `source` varchar(100)  NOT NULL DEFAULT ' '
);

CREATE TABLE `finnhub_stock_profile` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(20) DEFAULT '',
  `Exchange` varchar(20) DEFAULT 'US',
  `Description` varchar(200) DEFAULT '',
  `DisplaySymbol` varchar(20) DEFAULT '',
  `Type` varchar(25) DEFAULT '',
  `Mic` varchar(20) DEFAULT '',
  `Figi` varchar(20) DEFAULT '',
  `Currency` varchar(20) DEFAULT '',
  `Address` varchar(100) DEFAULT '',
  `City` varchar(20) DEFAULT '',
  `Country` varchar(20) DEFAULT '',
  `cusip` varchar(20) DEFAULT '',
  `sedol` varchar(20) DEFAULT '',
  `EmployeeTotal` int DEFAULT '0',
  `ggroup` varchar(45) DEFAULT '',
  `gind` varchar(45) DEFAULT '',
  `gsector` varchar(45) DEFAULT '',
  `gsubind` varchar(45) DEFAULT '',
  `IPODate` varchar(20) DEFAULT '',
  `isin` varchar(45) DEFAULT '',
  `MarketCapitalization` decimal(25,6) DEFAULT '0.000000',
  `naics` varchar(45) DEFAULT '',
  `naicsNationalIndustry` varchar(100) DEFAULT '',
  `naicsSector` varchar(100) DEFAULT '',
  `naicsSubsector` varchar(100) DEFAULT '',
  `Name` varchar(200) DEFAULT '',
  `Phone` varchar(100) DEFAULT '',
  `ShareOutstanding` decimal(20,6) DEFAULT '0.000000',
  `state` varchar(45) DEFAULT '',
  `Ticker` varchar(45) DEFAULT '',
  `WebURL` varchar(150) DEFAULT '',
  `Logo` varchar(110) DEFAULT '',
  `FinnhubIndustry` varchar(100) DEFAULT '',
  `Peer` varchar(2000) DEFAULT '{}',
  `UpdateDate` varchar(8000) DEFAULT '{}'
);

CREATE TABLE `finnhub_stock_sec_filings` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `symbol` varchar(45) DEFAULT NULL,
  `accessNumber` varchar(100) DEFAULT NULL,
  `cik` int DEFAULT NULL,
  `form` varchar(100) DEFAULT NULL,
  `acceptedDate` int DEFAULT NULL,
  `filedDate` int DEFAULT NULL,
  `filingURL` varchar(200) DEFAULT NULL,
  `reportURL` varchar(200) DEFAULT NULL
);

CREATE TABLE `index_nasdaq100` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `ExpiredDate` int DEFAULT '20990101'
);

CREATE TABLE `index_nasdaq100_200ma_updown_rate` (
  `Date` int NOT NULL,
  `Rate` int DEFAULT NULL
);

CREATE TABLE `naics_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Naics` varchar(45) DEFAULT ' ',
  `NationalIndustry` varchar(45) DEFAULT ' ',
  `Sector` varchar(45) DEFAULT ' ',
  `SubSector` varchar(45) DEFAULT ' '
);

CREATE TABLE `sic_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` int DEFAULT '0',
  `Industry` varchar(45) DEFAULT ' ',
  `Sector` varchar(45) DEFAULT ' '
);

CREATE TABLE `stock_splits` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '',
  `Date` int DEFAULT '19800101',
  `SplitFactor` decimal(10,5) DEFAULT '1.00000'
);

CREATE TABLE `tiingo_company_financial_state` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Exchange` varchar(45) DEFAULT NULL,
  `YearQuarter` int DEFAULT NULL,
  `accoci` decimal(30,5) DEFAULT NULL,
  `acctPay` decimal(30,5) DEFAULT NULL,
  `acctRec` decimal(30,5) DEFAULT NULL,
  `assetsCurrent` decimal(30,5) DEFAULT NULL,
  `assetsNonCurrent` decimal(30,5) DEFAULT NULL,
  `assetTurnover` decimal(30,5) DEFAULT NULL,
  `bookVal` decimal(30,5) DEFAULT NULL,
  `businessAcqDisposals` decimal(30,5) DEFAULT NULL,
  `bvps` decimal(30,5) DEFAULT NULL,
  `capex` decimal(30,5) DEFAULT NULL,
  `cashAndEq` decimal(30,5) DEFAULT NULL,
  `consolidatedIncome` decimal(30,5) DEFAULT NULL,
  `costRev` decimal(30,5) DEFAULT NULL,
  `currentRatio` decimal(30,5) DEFAULT NULL,
  `debt` decimal(30,5) DEFAULT NULL,
  `debtCurrent` decimal(30,5) DEFAULT NULL,
  `debtEquity` decimal(30,5) DEFAULT NULL,
  `debtNonCurrent` decimal(30,5) DEFAULT NULL,
  `deferredRev` decimal(30,5) DEFAULT NULL,
  `depamor` decimal(30,5) DEFAULT NULL,
  `deposits` decimal(30,5) DEFAULT NULL,
  `ebit` decimal(30,5) DEFAULT NULL,
  `ebitda` decimal(30,5) DEFAULT NULL,
  `ebt` decimal(30,5) DEFAULT NULL,
  `enterpriseVal` decimal(30,5) DEFAULT NULL,
  `eps` decimal(30,5) DEFAULT NULL,
  `epsDil` decimal(30,5) DEFAULT NULL,
  `epsQoQ` decimal(30,5) DEFAULT NULL,
  `equity` decimal(30,5) DEFAULT NULL,
  `freeCashFlow` decimal(30,5) DEFAULT NULL,
  `fxRate` decimal(30,5) DEFAULT NULL,
  `grossMargin` decimal(30,5) DEFAULT NULL,
  `grossProfit` decimal(30,5) DEFAULT NULL,
  `intangibles` decimal(30,5) DEFAULT NULL,
  `intexp` decimal(30,5) DEFAULT NULL,
  `inventory` decimal(30,5) DEFAULT NULL,
  `investments` decimal(30,5) DEFAULT NULL,
  `investmentsAcqDisposals` decimal(30,5) DEFAULT NULL,
  `investmentsCurrent` decimal(30,5) DEFAULT NULL,
  `investmentsNonCurrent` decimal(30,5) DEFAULT NULL,
  `issrepayDebt` decimal(30,5) DEFAULT NULL,
  `issrepayEquity` decimal(30,5) DEFAULT NULL,
  `liabilitiesCurrent` decimal(30,5) DEFAULT NULL,
  `liabilitiesNonCurrent` decimal(30,5) DEFAULT NULL,
  `longTermDebtEquity` decimal(30,5) DEFAULT NULL,
  `marketCap` decimal(30,5) DEFAULT NULL,
  `ncf` decimal(30,5) DEFAULT NULL,
  `ncff` decimal(30,5) DEFAULT NULL,
  `ncfi` decimal(30,5) DEFAULT NULL,
  `ncfo` decimal(30,5) DEFAULT NULL,
  `ncfx` decimal(30,5) DEFAULT NULL,
  `netinc` decimal(30,5) DEFAULT NULL,
  `netIncComStock` decimal(30,5) DEFAULT NULL,
  `netIncDiscOps` decimal(30,5) DEFAULT NULL,
  `netMargin` decimal(30,5) DEFAULT NULL,
  `nonControllingInterests` decimal(30,5) DEFAULT NULL,
  `opex` decimal(30,5) DEFAULT NULL,
  `opinc` decimal(30,5) DEFAULT NULL,
  `opMargin` decimal(30,5) DEFAULT NULL,
  `payDiv` decimal(30,5) DEFAULT NULL,
  `pbRatio` decimal(30,5) DEFAULT NULL,
  `peRatio` decimal(30,5) DEFAULT NULL,
  `piotroskiFScore` decimal(30,5) DEFAULT NULL,
  `ppeq` decimal(30,5) DEFAULT NULL,
  `prefDVDs` decimal(30,5) DEFAULT NULL,
  `profitMargin` decimal(30,5) DEFAULT NULL,
  `retainedEarnings` decimal(30,5) DEFAULT NULL,
  `revenue` decimal(30,5) DEFAULT NULL,
  `revenueQoQ` decimal(30,5) DEFAULT NULL,
  `rnd` decimal(30,5) DEFAULT NULL,
  `roa` decimal(30,5) DEFAULT NULL,
  `roe` decimal(30,5) DEFAULT NULL,
  `rps` decimal(30,5) DEFAULT NULL,
  `sbcomp` decimal(30,5) DEFAULT NULL,
  `sga` decimal(30,5) DEFAULT NULL,
  `shareFactor` decimal(30,5) DEFAULT NULL,
  `sharesBasic` decimal(30,5) DEFAULT NULL,
  `shareswa` decimal(30,5) DEFAULT NULL,
  `shareswaDil` decimal(30,5) DEFAULT NULL,
  `taxAssets` decimal(30,5) DEFAULT NULL,
  `taxExp` decimal(30,5) DEFAULT NULL,
  `taxLiabilities` decimal(30,5) DEFAULT NULL,
  `totalAssets` decimal(30,5) DEFAULT NULL,
  `totalLiabilities` decimal(30,5) DEFAULT NULL,
  `trailingPEG1Y` decimal(30,5) DEFAULT NULL
);

CREATE TABLE `tiingo_crypto_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '' COMMENT 'Ticker = BaseCurrency + QuoteCurrency',
  `BaseCurrency` varchar(20) DEFAULT '',
  `QuoteCurrency` varchar(20) DEFAULT '',
  `Name` varchar(100) DEFAULT '',
  `Description` varchar(100) DEFAULT '',
  `UpdateDate` varchar(5000) DEFAULT ''
);

CREATE TABLE `tiingo_fundamental_definitions` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `dataCode` varchar(45) DEFAULT '',
  `name` varchar(100) DEFAULT '',
  `description` varchar(4000) DEFAULT '‘’',
  `statementType` varchar(45) DEFAULT '',
  `units` varchar(45) DEFAULT ''
);

CREATE TABLE `tiingo_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Industry` varchar(100) DEFAULT NULL,
  `Sector` varchar(100) DEFAULT NULL
);

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
  `publishedDate` varchar(100) DEFAULT NULL
);

CREATE TABLE `tiingo_stock_52week_high` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Exchange` varchar(50) DEFAULT NULL,
  `Date` int DEFAULT NULL
);

CREATE TABLE `tiingo_stock_52week_low` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Exchange` varchar(50) DEFAULT NULL,
  `Date` int DEFAULT NULL
);

CREATE TABLE `tiingo_stock_current_trace` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Date` int DEFAULT NULL,
  `SICCode` int DEFAULT NULL
);

CREATE TABLE `tiingo_stock_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int NOT NULL DEFAULT '19800101',
  `Exchange` varchar(100) NOT NULL DEFAULT ' ',
  `Symbol` varchar(45) NOT NULL DEFAULT ' ',
  `LastClose` decimal(17,6) NOT NULL DEFAULT '0.000000',
  `Open` decimal(17,6) NOT NULL DEFAULT '0.000000',
  `High` decimal(17,6) NOT NULL DEFAULT '0.000000',
  `Low` decimal(17,6) NOT NULL DEFAULT '0.000000',
  `Close` decimal(17,6) NOT NULL DEFAULT '0.000000',
  `Volume` decimal(20,0) NOT NULL DEFAULT '0',
  `Amount` decimal(25,0) NOT NULL DEFAULT '0',
  `Dividend` decimal(10,3) NOT NULL DEFAULT '0.000',
  `SplitFactor` decimal(10,5) NOT NULL DEFAULT '1.00000',
  `UpAndDown` decimal(10,3) NOT NULL DEFAULT '0.000',
  `UpDownRate` decimal(10,3) NOT NULL DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) NOT NULL DEFAULT '0.000',
  `TotalValue` decimal(20,0) NOT NULL DEFAULT '0',
  `CurrentValue` decimal(20,0) NOT NULL DEFAULT '0'
);

CREATE TABLE `tiingo_stock_delisted_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Date` int DEFAULT NULL
);

CREATE TABLE `tiingo_stock_new_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Date` int DEFAULT NULL
);

CREATE TABLE `tiingo_stock_profile` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `TiingoPermaTicker` varchar(45) DEFAULT '',
  `Symbol` varchar(12) DEFAULT '',
  `Name` varchar(200) DEFAULT '',
  `IsActive` tinyint DEFAULT NULL,
  `IsADR` tinyint DEFAULT NULL,
  `TiingoIndustry` varchar(100) DEFAULT '',
  `TiingoSector` varchar(100) DEFAULT '',
  `SICCode` int DEFAULT '0',
  `SICIndustry` varchar(100) DEFAULT '',
  `SICSector` varchar(100) DEFAULT '',
  `ReportingCurrency` varchar(45) DEFAULT '',
  `Location` varchar(100) DEFAULT '',
  `CompanyWebSite` varchar(100) DEFAULT '',
  `SECFilingWebSite` varchar(150) DEFAULT '',
  `UpdateDate` varchar(5000) DEFAULT '{}'
);

CREATE TABLE `world_choice_crypto` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT ''
);

CREATE TABLE `world_choice_forex` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT ''
);

CREATE TABLE `world_choice_stock` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT ''
);

CREATE TABLE `world_market_option` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `FinnhubToken` varchar(100) DEFAULT '',
  `FinnhubMaxPerHour` int DEFAULT '3000',
  `TiingoToken` varchar(100) DEFAULT '',
  `TiingoMaxPerHour` int DEFAULT '450'
);

-- Dump completed on 2026-07-20 19:26:55
