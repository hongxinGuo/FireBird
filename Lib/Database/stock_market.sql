-- MySQL dump 10.13  Distrib 8.0.19, for Win64 (x86_64)
--
-- Host: localhost    Database: stock_market
-- ------------------------------------------------------
-- Server version	8.4.8

--
-- Table structure for table `china_choice_stock`
--

CREATE TABLE `china_choice_stock` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(10) DEFAULT NULL
);

--
-- Table structure for table `finnhub_company_exective`
--

CREATE TABLE `finnhub_company_exective` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `CompanySymbol` varchar(45) DEFAULT NULL,
  `Age` int DEFAULT NULL,
  `Compensation` int DEFAULT NULL,
  `Currency` varchar(20) DEFAULT NULL,
  `Name` varchar(45) DEFAULT NULL,
  `Position` varchar(100) DEFAULT NULL,
  `Sex` varchar(20) DEFAULT NULL,
  `Since` int DEFAULT NULL
);

--
-- Table structure for table `tiingo_market_news`
--

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

--
-- Table structure for table `tiingo_stock_new_symbol`
--

CREATE TABLE `tiingo_stock_new_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Date` int DEFAULT NULL
);

--
-- Table structure for table `index_nasdaq100_200ma_updown_rate`
--


CREATE TABLE `index_nasdaq100_200ma_updown_rate` (
  `Date` int NOT NULL,
  `Rate` int DEFAULT NULL
);

--
-- Table structure for table `world_choice_stock`
--

CREATE TABLE `world_choice_stock` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45)
);

--
-- Table structure for table `tiingo_company_financial_state`
--

CREATE TABLE `tiingo_company_financial_state` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
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
  `trailingPEG1Y` varchar(45) DEFAULT NULL
);

--
-- Table structure for table `finnhub_crypto_exchange`
--

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
  `MyUnknownColumn` text
) ;

--
-- Table structure for table `finnhub_company_news`
--


CREATE TABLE `finnhub_company_news` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45)  DEFAULT NULL,
  `Category` varchar(100)  DEFAULT NULL,
  `DateTime` decimal(30,0) DEFAULT NULL,
  `Headline` varchar(1000) DEFAULT NULL,
  `NewsID` int DEFAULT NULL,
  `Image` varchar(1000) DEFAULT NULL,
  `RelatedSymbol` varchar(45)  DEFAULT NULL,
  `Source` varchar(500) DEFAULT NULL,
  `Summary` varchar(10000)  DEFAULT NULL,
  `URL` varchar(1000)  DEFAULT NULL
);


--
-- Table structure for table `finnhub_insider_transaction`
--


CREATE TABLE `finnhub_insider_transaction` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `PersonName` varchar(200) DEFAULT NULL,
  `Share` decimal(20,0) DEFAULT NULL,
  `Change` decimal(20,0) DEFAULT NULL,
  `FilingDate` int DEFAULT NULL,
  `TransactionDate` int DEFAULT NULL,
  `TransactionCode` varchar(5) DEFAULT NULL,
  `TransactionPrice` double DEFAULT NULL
);


--
-- Table structure for table `tiingo_stock_52week_high`
--


CREATE TABLE `tiingo_stock_52week_high` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Exchange` varchar(50) DEFAULT NULL,
  `Date` int DEFAULT NULL
);


--
-- Table structure for table `sic_industry`
--


CREATE TABLE `sic_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` int DEFAULT '0',
  `Industry` varchar(45) DEFAULT ' ',
  `Sector` varchar(45) DEFAULT ' '
);


--
-- Table structure for table `finnhub_forex_symbol`
--


CREATE TABLE `finnhub_forex_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Description` varchar(100) DEFAULT ' ',
  `DisplaySymbol` varchar(100) DEFAULT ' ',
  `Exchange` varchar(100) DEFAULT ' ',
  `Symbol` varchar(100) DEFAULT ' ',
  `IPOStatus` int DEFAULT '128',
  `UpdateDate` varchar(10000) DEFAULT ''
);
/;

--
-- Table structure for table `naics_industry`
--


CREATE TABLE `naics_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Naics` varchar(45) DEFAULT ' ',
  `NationalIndustry` varchar(45) DEFAULT ' ',
  `Sector` varchar(45) DEFAULT ' ',
  `SubSector` varchar(45) DEFAULT ' '
);


--
-- Table structure for table `china_stock_weekline`
--


CREATE TABLE `china_stock_weekline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
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

--
-- Table structure for table `finnhub_insider_sentiment`
--

CREATE TABLE `finnhub_insider_sentiment` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Date` int DEFAULT NULL,
  `Change` int DEFAULT NULL,
  `mspr` double DEFAULT NULL
);

--
-- Table structure for table `tiingo_stock_dayline`
--

CREATE TABLE `tiingo_stock_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19800101',
  `Exchange` varchar(100) DEFAULT ' ',
  `Symbol` varchar(45) DEFAULT ' ',
  `LastClose` decimal(17,6) DEFAULT '0.000000',
  `Open` decimal(17,6) DEFAULT '0.000000',
  `High` decimal(17,6) DEFAULT '0.000000',
  `Low` decimal(17,6) DEFAULT '0.000000',
  `Close` decimal(17,6) DEFAULT '0.000000',
  `Volume` decimal(20,0) DEFAULT '0',
  `Amount` decimal(25,0) DEFAULT '0',
  `Dividend` decimal(10,3) DEFAULT '0.000',
  `SplitFactor` decimal(10,5) DEFAULT '1.00000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0'
);

--
-- Table structure for table `world_choice_crypto`
--

CREATE TABLE `world_choice_crypto` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT ''
);

--
-- Table structure for table `finnhub_crypto_symbol`
--

CREATE TABLE `finnhub_crypto_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Description` varchar(100) DEFAULT ' ',
  `DisplaySymbol` varchar(100) DEFAULT ' ',
  `Exchange` varchar(100) DEFAULT ' ',
  `Symbol` varchar(100) DEFAULT ' ',
  `IPOStatus` int DEFAULT '128',
  `UpdateDate` varchar(10000) DEFAULT ''
);

--
-- Table structure for table `finnhub_economic_calendar`
--

CREATE TABLE `finnhub_economic_calendar` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Time` varchar(45) DEFAULT ' ',
  `Country` varchar(45) DEFAULT ' ',
  `Event` varchar(100) DEFAULT ' ',
  `Impact` varchar(45) DEFAULT ' ',
  `Actual` decimal(10,3) DEFAULT '0.000',
  `Estimate` decimal(10,3) DEFAULT '0.000',
  `Prev` decimal(10,3) DEFAULT '0.000',
  `Unit` varchar(45) DEFAULT ' '
);

--
-- Table structure for table `finnhub_forex_exchange`
--

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
  `MyUnknownColumn` text
);


--
-- Table structure for table `china_stock_dayline`
--


CREATE TABLE `china_stock_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
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

--
-- Table structure for table `index_nasdaq100`
--


CREATE TABLE `index_nasdaq100` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `ExpiredDate` int DEFAULT '20990101'
);


--
-- Table structure for table `finnhub_crypto_dayline`
--


CREATE TABLE `finnhub_crypto_dayline` (
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
  `SplitFactor` decimal(10,5) DEFAULT '1.00000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0'
);


--
-- Table structure for table `finnhub_stock_sec_filings`
--


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


--
-- Table structure for table `finnhub_country_list`
--


CREATE TABLE `finnhub_country_list` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code2` varchar(45) DEFAULT ' ',
  `Code3` varchar(45) DEFAULT ' ',
  `CodeNo` varchar(45) DEFAULT ' ',
  `Country` varchar(100) DEFAULT ' ',
  `Currency` varchar(45) DEFAULT ' ',
  `CurrencyCode` varchar(45) DEFAULT ' '
);


--
-- Table structure for table `tiingo_stock_52week_low`
--


CREATE TABLE `tiingo_stock_52week_low` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Exchange` varchar(50) DEFAULT NULL,
  `Date` int DEFAULT NULL
);

--
-- Table structure for table `tiingo_stock_current_trace`
--


CREATE TABLE `tiingo_stock_current_trace` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Date` int DEFAULT NULL,
  `SICCode` int DEFAULT NULL
)i;


--
-- Table structure for table `finnhub_stock_dayline`
--

CREATE TABLE `finnhub_stock_dayline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19800101',
  `Exchange` varchar(100) DEFAULT ' ',
  `Symbol` varchar(45) DEFAULT ' ',
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

--
-- Table structure for table `tiingo_industry`
--


CREATE TABLE `tiingo_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Industry` varchar(100) DEFAULT NULL,
  `Sector` varchar(100) DEFAULT NULL
);

--
-- Table structure for table `china_market_options`
--


CREATE TABLE `china_market_options` (
  `LastLoginDate` int DEFAULT '19900101',
  `LastLoginTime` int DEFAULT '0'
);


--
-- Table structure for table `stock_splits`
--


CREATE TABLE `stock_splits` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT '',
  `Date` int DEFAULT '19800101',
  `SplitFactor` decimal(10,5) DEFAULT '1.00000'
);

--
-- Table structure for table `world_market_option`
--

CREATE TABLE `world_market_option` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `FinnhubToken` varchar(100) DEFAULT '',
  `FinnhubMaxPerHour` int DEFAULT '3000',
  `TiingoToken` varchar(100) DEFAULT '',
  `TiingoMaxPerHour` int DEFAULT '450',
  `QuandlToken` varchar(100) DEFAULT '',
  `QuandlMaxPerHour` int DEFAULT '100'
);

--
-- Table structure for table `china_stock_code`
--

CREATE TABLE `china_stock_code` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Exchange` varchar(100) DEFAULT ' ',
  `Description` varchar(100) DEFAULT ' ',
  `Symbol` varchar(12) DEFAULT ' ',
  `DisplaySymbol` varchar(12) DEFAULT ' ',
  `IPOStatus` int DEFAULT '128',
  `UpdateDate` varchar(10000) DEFAULT ''
);

--
-- Table structure for table `world_choice_forex`
--


CREATE TABLE `world_choice_forex` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT ''
);


--
-- Table structure for table `finnhub_stock_exchange`
--


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
  `MyUnknownColumn` text
);


--
-- Table structure for table `tiingo_stock_fundamental`
--

CREATE TABLE `tiingo_stock_fundamental` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `TiingoPermaTicker` varchar(45) DEFAULT '',
  `Ticker` varchar(45) DEFAULT '',
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
  `IPOStatus` int DEFAULT '128',
  `UpdateDate` varchar(10000) DEFAULT '{}'
);


--
-- Table structure for table `china_current_weekline`
--


CREATE TABLE `china_current_weekline` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int DEFAULT '19900101',
  `Exchange` varchar(45) DEFAULT '',
  `Symbol` varchar(10) DEFAULT ' ',
  `LastClose` decimal(10,3) DEFAULT '0.000',
  `Open` decimal(10,3) DEFAULT '0.000',
  `High` decimal(10,3) DEFAULT '0.000',
  `Low` decimal(10,3) DEFAULT '0.000',
  `Close` decimal(10,3) DEFAULT '0.000',
  `Volume` decimal(18,0) DEFAULT '0',
  `Amount` decimal(20,0) DEFAULT '0',
  `Dividend` decimal(10,3) DEFAULT '0.000',
  `SplitFactor` decimal(10,5) DEFAULT '1.00000',
  `UpAndDown` decimal(10,3) DEFAULT '0.000',
  `UpDownRate` decimal(10,3) DEFAULT '0.000',
  `ChangeHandRate` decimal(10,3) DEFAULT '0.000',
  `TotalValue` decimal(20,0) DEFAULT '0',
  `CurrentValue` decimal(20,0) DEFAULT '0'
);


--
-- Table structure for table `finnhub_stock_profile`
--


CREATE TABLE `finnhub_stock_profile` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(20) DEFAULT '',
  `ExchangeCode` varchar(3) DEFAULT 'US',
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
  `IPOStatus` int DEFAULT '128',
  `UpdateDate` varchar(10000) DEFAULT '{}' 
);


--
-- Table structure for table `tiingo_stock_delisted_symbol`
--


CREATE TABLE `tiingo_stock_delisted_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT NULL,
  `Date` int DEFAULT NULL
);


--
-- Table structure for table `tiingo_crypto_symbol`
--


CREATE TABLE `tiingo_crypto_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Ticker` varchar(45) DEFAULT '',
  `BaseCurrency` varchar(20) DEFAULT '',
  `QuoteCurrency` varchar(20) DEFAULT '',
  `Name` varchar(100) DEFAULT '',
  `Description` varchar(100) DEFAULT '',
  `UpdateDate` varchar(10000) DEFAULT ''
);


--
-- Table structure for table `tiingo_fundamental_definitions`
--


CREATE TABLE `tiingo_fundamental_definitions` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `dataCode` varchar(45) DEFAULT '',
  `name` varchar(100) DEFAULT '',
  `description` mediumtext,
  `statementType` varchar(45) DEFAULT '',
  `units` varchar(45) DEFAULT ''
) ;

--
-- Table structure for table `china_stock_code_section`
--


CREATE TABLE `china_stock_code_section` (
  `ID` int NOT NULL,
  `Active` tinyint DEFAULT NULL,
  `Market` int DEFAULT NULL,
  `IndexNumber` int DEFAULT NULL,
  `Comment` varchar(200) DEFAULT NULL
);


--
-- Table structure for table `finnhub_forex_dayline`
--


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

--
-- Table structure for table `finnhub_stock_estimates_eps_surprise`
--

CREATE TABLE `finnhub_stock_estimates_eps_surprise` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) DEFAULT ' ',
  `Date` int DEFAULT '0',
  `Actual` double DEFAULT '0',
  `Estimate` double DEFAULT '0'
);


-- Dump completed on 2026-05-22  4:37:42
