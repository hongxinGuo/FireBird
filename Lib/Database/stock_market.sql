-- MySQL dump 10.13  Distrib 8.0.19, for Win64 (x86_64)
--
-- Host: localhost    Database: stock_market
-- ------------------------------------------------------
-- Server version	9.7.1

CREATE TABLE `alpaca_stock_symbol` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Exchange` varchar(45) NOT NULL DEFAULT'‘ ’',
  `Symbol` varchar(45) NOT NULL DEFAULT '‘ ’',
  `Name` varchar(1000) NOT NULL DEFAULT '‘ ’'
);

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
  `Symbol` varchar(45)  NOT NULL DEFAULT ' ',
  `Date` int NOT NULL DEFAULT '0',
  `Change` int NOT NULL DEFAULT '0',
  `mspr` double NOT NULL DEFAULT '0'
);

CREATE TABLE `finnhub_insider_transaction` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45)  NOT NULL DEFAULT '‘ ’',
  `PersonName` varchar(200)  NOT NULL DEFAULT '‘ ’',
  `Share` decimal(20,0) NOT NULL DEFAULT '0',
  `ShareChange` decimal(20,0) NOT NULL DEFAULT '0',
  `FilingDate` int NOT NULL DEFAULT '0',
  `TransactionDate` int NOT NULL DEFAULT '0',
  `TransactionCode` varchar(5)  NOT NULL DEFAULT '‘ ’',
  `TransactionPrice` double NOT NULL DEFAULT '0'
);

CREATE TABLE `finnhub_stock_dayline` (
   `ID` int NOT NULL AUTO_INCREMENT,
  `Date` int NOT NULL DEFAULT '19800101',
  `Exchange` varchar(100)  NOT NULL DEFAULT ' ',
  `Symbol` varchar(45)  NOT NULL DEFAULT ' ',
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
  `Symbol` varchar(45)  NOT NULL DEFAULT ' ',
  `Date` int NOT NULL DEFAULT '0',
  `Actual` double NOT NULL DEFAULT '0',
  `Estimate` double NOT NULL DEFAULT '0'
);

CREATE TABLE `finnhub_stock_exchange` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `code` varchar(45)  NOT NULL DEFAULT '‘ ’',
  `name` varchar(100)  NOT NULL DEFAULT '‘ ’',
  `mic` varchar(45)  NOT NULL DEFAULT '‘ ’',
  `timezone` varchar(45)  NOT NULL DEFAULT '‘ ’',
  `pre_market` varchar(45)  NOT NULL DEFAULT '‘ ‘',
  `hour` varchar(45)  NOT NULL DEFAULT '’ ‘',
  `post_market` varchar(45)  NOT NULL DEFAULT '’ ‘',
  `close_date` varchar(45)  NOT NULL DEFAULT '’ ‘',
  `country` varchar(45)  NOT NULL DEFAULT '’ ‘',
  `country_name` varchar(100)  NOT NULL DEFAULT '’ ‘',
  `source` varchar(100)  NOT NULL DEFAULT '’ ‘'
);

CREATE TABLE `finnhub_stock_profile` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(20)  NOT NULL DEFAULT ' ',
  `Exchange` varchar(20)  NOT NULL DEFAULT 'US',
  `Description` varchar(200)  NOT NULL DEFAULT ' ',
  `DisplaySymbol` varchar(20)  NOT NULL DEFAULT ' ',
  `Type` varchar(25)  NOT NULL DEFAULT ' ',
  `Mic` varchar(20)  NOT NULL DEFAULT ' ',
  `Figi` varchar(20)  NOT NULL DEFAULT ' ',
  `Currency` varchar(20)  NOT NULL DEFAULT ' ',
  `Address` varchar(100)  NOT NULL DEFAULT ' ',
  `City` varchar(20)  NOT NULL DEFAULT ' ',
  `Country` varchar(20)  NOT NULL DEFAULT ' ',
  `cusip` varchar(20)  NOT NULL DEFAULT ' ',
  `sedol` varchar(20)  NOT NULL DEFAULT ' ',
  `EmployeeTotal` int NOT NULL DEFAULT '0',
  `ggroup` varchar(45)  NOT NULL DEFAULT ' ',
  `gind` varchar(45)  NOT NULL DEFAULT ' ',
  `gsector` varchar(45)  NOT NULL DEFAULT ' ',
  `gsubind` varchar(45)  NOT NULL DEFAULT ' ',
  `IPODate` varchar(20)  NOT NULL DEFAULT ' ',
  `isin` varchar(45)  NOT NULL DEFAULT ' ',
  `MarketCapitalization` decimal(25,6) NOT NULL DEFAULT '0.000000',
  `naics` varchar(45)  NOT NULL DEFAULT ' ',
  `naicsNationalIndustry` varchar(100)  NOT NULL DEFAULT ' ',
  `naicsSector` varchar(100)  NOT NULL DEFAULT ' ',
  `naicsSubsector` varchar(100)  NOT NULL DEFAULT ' ',
  `Name` varchar(200)  NOT NULL DEFAULT ' ',
  `Phone` varchar(100)  NOT NULL DEFAULT ' ',
  `ShareOutstanding` decimal(20,6) NOT NULL DEFAULT '0.000000',
  `state` varchar(45)  NOT NULL DEFAULT ' ',
  `Ticker` varchar(45)  NOT NULL DEFAULT ' ',
  `WebURL` varchar(200)  NOT NULL DEFAULT ' ',
  `Logo` varchar(200)  NOT NULL DEFAULT ' ',
  `FinnhubIndustry` varchar(200)  NOT NULL DEFAULT ' ',
  `Peer` varchar(1000)  NOT NULL DEFAULT '{}',
  `UpdateDate` varchar(4000)  NOT NULL DEFAULT '{}'
);

CREATE TABLE `finnhub_stock_sec_filings` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `symbol` varchar(45)  NOT NULL DEFAULT '‘ ’',
  `accessNumber` varchar(100)  NOT NULL DEFAULT '‘ ’',
  `cik` int NOT NULL DEFAULT '0',
  `form` varchar(100)  NOT NULL DEFAULT '‘ ’',
  `acceptedDate` int NOT NULL DEFAULT '0',
  `filedDate` int NOT NULL DEFAULT '0',
  `filingURL` varchar(200)  NOT NULL DEFAULT '‘ ’',
  `reportURL` varchar(200)  NOT NULL DEFAULT '‘ ’'
);

CREATE TABLE `index_nasdaq100` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45)  NOT NULL DEFAULT '‘ ’',
  `ExpiredDate` int NOT NULL DEFAULT '20990101'
);

CREATE TABLE `index_nasdaq100_200ma_updown_rate` (
  `Date` int NOT NULL,
  `Rate` int NOT NULL DEFAULT '0'
);

CREATE TABLE `naics_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Naics` varchar(45)  NOT NULL DEFAULT ' ',
  `NationalIndustry` varchar(45)  NOT NULL DEFAULT ' ',
  `Sector` varchar(45)  NOT NULL DEFAULT ' ',
  `SubSector` varchar(45)  NOT NULL DEFAULT ' '
);

CREATE TABLE `sic_industry` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` int NOT NULL DEFAULT '0',
  `Industry` varchar(45)  NOT NULL DEFAULT ' ',
  `Sector` varchar(45)  NOT NULL DEFAULT ' '
);

CREATE TABLE `stock_splits` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45)  NOT NULL DEFAULT ' ',
  `Date` int NOT NULL DEFAULT '19800101',
  `SplitFactor` decimal(10,5) NOT NULL DEFAULT '1.00000'
);

CREATE TABLE `tiingo_company_financial_state` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) NOT NULL DEFAULT '‘ ’',
  `Exchange` varchar(45) NOT NULL DEFAULT '‘ ’',
  `YearQuarter` int NOT NULL DEFAULT '0',
  `accoci` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `acctPay` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `acctRec` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `assetsCurrent` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `assetsNonCurrent` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `assetTurnover` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `bookVal` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `businessAcqDisposals` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `bvps` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `capex` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `cashAndEq` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `consolidatedIncome` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `costRev` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `currentRatio` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `debt` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `debtCurrent` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `debtEquity` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `debtNonCurrent` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `deferredRev` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `depamor` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `deposits` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `ebit` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `ebitda` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `ebt` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `enterpriseVal` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `eps` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `epsDil` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `epsQoQ` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `equity` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `freeCashFlow` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `fxRate` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `grossMargin` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `grossProfit` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `intangibles` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `intexp` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `inventory` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `investments` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `investmentsAcqDisposals` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `investmentsCurrent` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `investmentsNonCurrent` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `issrepayDebt` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `issrepayEquity` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `liabilitiesCurrent` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `liabilitiesNonCurrent` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `longTermDebtEquity` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `marketCap` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `ncf` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `ncff` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `ncfi` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `ncfo` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `ncfx` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `netinc` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `netIncComStock` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `netIncDiscOps` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `netMargin` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `nonControllingInterests` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `opex` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `opinc` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `opMargin` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `payDiv` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `pbRatio` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `peRatio` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `piotroskiFScore` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `ppeq` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `prefDVDs` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `profitMargin` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `retainedEarnings` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `revenue` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `revenueQoQ` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `rnd` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `roa` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `roe` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `rps` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `sbcomp` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `sga` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `shareFactor` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `sharesBasic` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `shareswa` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `shareswaDil` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `taxAssets` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `taxExp` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `taxLiabilities` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `totalAssets` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `totalLiabilities` decimal(30,5) NOT NULL DEFAULT '0.00000',
  `trailingPEG1Y` decimal(30,5) NOT NULL DEFAULT '0.00000'
);

CREATE TABLE `tiingo_crypto_symbol` (
 `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) NOT NULL DEFAULT ' ',
  `BaseCurrency` varchar(20) NOT NULL DEFAULT ' ',
  `QuoteCurrency` varchar(20) NOT NULL DEFAULT ' ',
  `Name` varchar(100) NOT NULL DEFAULT ' ',
  `Description` varchar(100) NOT NULL DEFAULT ' ',
  `UpdateDate` varchar(5000) NOT NULL DEFAULT ' '
);

CREATE TABLE `tiingo_fundamental_definitions` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `dataCode` varchar(45) NOT NULL DEFAULT ' ',
  `name` varchar(100) NOT NULL DEFAULT ' ',
  `description` varchar(4000) NOT NULL DEFAULT ' ',
  `statementType` varchar(45) NOT NULL DEFAULT ' ',
  `units` varchar(45) NOT NULL DEFAULT ' '
);

CREATE TABLE `tiingo_industry` (
   `ID` int NOT NULL AUTO_INCREMENT,
  `Industry` varchar(100) NOT NULL DEFAULT '‘ ’',
  `Sector` varchar(100) NOT NULL DEFAULT '‘ ’'
);

CREATE TABLE `tiingo_market_news` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `ids` int NOT NULL DEFAULT '0',
  `source` varchar(100) NOT NULL DEFAULT '‘ ’',
  `description` varchar(500) NOT NULL DEFAULT '‘ ’',
  `url` varchar(100) NOT NULL DEFAULT '‘ ’',
  `tags` varchar(400) NOT NULL DEFAULT '‘ ’',
  `tickers` varchar(200) NOT NULL DEFAULT '‘ ’',
  `title` varchar(100) NOT NULL DEFAULT '‘ ’',
  `crawlDate` varchar(100) NOT NULL DEFAULT '‘ ’',
  `publishedDate` varchar(100) NOT NULL DEFAULT '‘ ’'
);

CREATE TABLE `tiingo_stock_52week_high` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) NOT NULL DEFAULT '‘ ’',
  `Exchange` varchar(50) NOT NULL DEFAULT '‘ ’',
  `Date` int NOT NULL DEFAULT '0'
);

CREATE TABLE `tiingo_stock_52week_low` (
 `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) NOT NULL DEFAULT '‘ ’',
  `Exchange` varchar(50) NOT NULL DEFAULT '‘ ’',
  `Date` int NOT NULL DEFAULT '0'
);

CREATE TABLE `tiingo_stock_5years_low_70percent` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) NOT NULL DEFAULT '‘ ’',
  `Date` int NOT NULL DEFAULT '19700101'
);

CREATE TABLE `tiingo_stock_5years_low_80percent` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) NOT NULL DEFAULT '‘ ’',
  `Date` int NOT NULL DEFAULT '19700101'
);

CREATE TABLE `tiingo_stock_5years_low_90percent` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45) NOT NULL DEFAULT '‘ ’',
  `Date` int NOT NULL DEFAULT '19700101'
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
  `Symbol` varchar(45)  NOT NULL DEFAULT ' '
);

CREATE TABLE `world_choice_forex` (
 `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45)  NOT NULL DEFAULT ' '
);

CREATE TABLE `world_choice_stock` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Symbol` varchar(45)  NOT NULL DEFAULT ' '
);

CREATE TABLE `world_market_option` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `FinnhubToken` varchar(100)  NOT NULL DEFAULT ' ',
  `FinnhubMaxPerHour` int NOT NULL DEFAULT '3000',
  `TiingoToken` varchar(100)  NOT NULL DEFAULT ' ',
  `TiingoMaxPerHour` int NOT NULL DEFAULT '450'
);

-- Dump completed on 2026-07-20 19:26:55
