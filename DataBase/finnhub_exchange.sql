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
-- Dumping data for table `finnhub_stock_exchange`
--

LOCK TABLES `finnhub_stock_exchange` WRITE;
/*!40000 ALTER TABLE `finnhub_stock_exchange` DISABLE KEYS */;
INSERT INTO `finnhub_stock_exchange` VALUES (1,'AS','NYSE EURONEXT - EURONEXT AMSTERDAM','XAMS','Europe/Amsterdam','09:00-17:40','','','https://www.tradinghours.com/exchanges/euronext'),(2,'AT','ATHENS EXCHANGE S.A. CASH MARKET','ASEX','Europe/Athens','10:15-17:20','','','https://www.tradinghours.com/exchanges/ase-athens'),(3,'AX','ASX - ALL MARKETS','XASX','Australia/Sydney','10:00-16:00','','AU','https://www.tradinghours.com/exchanges/asx'),(4,'BA','BOLSA DE COMERCIO DE BUENOS AIRES','XBUE','America/Argentina/Buenos_Aires','10:30-17:15','','','https://www.tradinghours.com/exchanges/bcba'),(5,'BC','BOLSA DE VALORES DE COLOMBIA','XBOG','America/Cuiaba','09:15-16:00','','','https://www.tradinghours.com/exchanges/bvc'),(6,'BD','BUDAPEST STOCK EXCHANGE','XBUD','Europe/Budapest','08:15-17:20','','','https://www.tradinghours.com/exchanges/bse-budapest'),(7,'BE','BOERSE BERLIN','XBER','Europe/Berlin','08:00-20:00','','','https://www.tradinghours.com/exchanges/xber'),(8,'BK','STOCK EXCHANGE OF THAILAND','XBKK','Asia/Bangkok','09:30-17:00','','','https://www.tradinghours.com/exchanges/set'),(9,'BO','BSE LTD','XBOM','Asia/Kolkata','09:00-16:00','','IN','https://www.tradinghours.com/exchanges/bse-bombay'),(10,'BR','NYSE EURONEXT - EURONEXT BRUSSELS','XBRU','Europe/Brussels','09:00-17:30','','','https://www.tradinghours.com/exchanges/euronext-brussels'),(11,'CN','CANADIAN NATIONAL STOCK EXCHANGE','XCNQ','America/New_York','08:00-17:00','','','https://www.tradinghours.com/exchanges/cnsx'),(12,'CO','OMX NORDIC EXCHANGE COPENHAGEN A/S','XCSE','Europe/Copenhagen','09:00-17:00','','','https://www.tradinghours.com/exchanges/omxc-copenhagen'),(13,'CR','CARACAS STOCK EXCHANGE','BVCA','America/Caracas','08:30-13:30','','','https://www.tradinghours.com/exchanges/bvcc'),(14,'DB','DUBAI FINANCIAL MARKET','XDFM','Asia/Dubai','09:30-14:00','','','https://www.tradinghours.com/exchanges/dfm'),(15,'DE','XETRA','XETR','Europe/Berlin','09:00-17:30','','DE','https://www.tradinghours.com/exchanges/xetr'),(16,'DU','BOERSE DUESSELDORF','XDUS','Europe/Berlin','08:00:20:00','','','https://www.tradinghours.com/exchanges/xdus'),(17,'F','DEUTSCHE BOERSE AG','XFRA','Europe/Berlin','08:00-20:00','','DE','https://www.tradinghours.com/exchanges/fsx'),(18,'HE','NASDAQ OMX HELSINKI LTD','XHEL','Europe/Helsinki','10:00-18:30','','','https://www.tradinghours.com/exchanges/omxh-helsinki'),(19,'HK','HONG KONG EXCHANGES AND CLEARING LTD','XHKG','Asia/Hong_Kong','09:00-16:10','','','https://www.tradinghours.com/exchanges/hkex'),(20,'HM','HANSEATISCHE WERTPAPIERBOERSE HAMBURG','XHAM','Europe/Berlin','08:00-20:00','','','https://www.tradinghours.com/exchanges/xham'),(21,'IC','NASDAQ OMX ICELAND','XICE','Atlantic/Reykjavik','09:30-15:30','','','https://www.tradinghours.com/exchanges/xice'),(22,'IR','IRISH STOCK EXCHANGE - ALL MARKET','XDUB','Europe/Dublin','09:00-17:30','','','https://www.tradinghours.com/exchanges/ise'),(23,'IS','BORSA ISTANBUL','XIST','Europe/Istanbul','09:40-18:10','','','https://www.tradinghours.com/exchanges/bist'),(24,'JK','INDONESIA STOCK EXCHANGE','XIDX','Asia/Jakarta','08:45-15:15','','','https://www.tradinghours.com/exchanges/idx'),(25,'JO','JOHANNESBURG STOCK EXCHANGE','XJSE','Africa/Johannesburg','09:00-17:00','','','https://www.tradinghours.com/exchanges/jse'),(26,'KL','BURSA MALAYSIA','XKLS','Asia/Kuala_Lumpur','08:30-17:00','','','https://www.tradinghours.com/exchanges/myx'),(27,'KQ','KOREA EXCHANGE (KOSDAQ)','XKOS','Asia/Seoul','09:00-15:30','','KP','https://www.tradinghours.com/exchanges/kosdaq'),(28,'KS','KOREA EXCHANGE (STOCK MARKET)','XKRX','Asia/Seoul','08:00-18:00','','KP','https://www.tradinghours.com/exchanges/krx'),(29,'L','LONDON STOCK EXCHANGE','XLON','Europe/London','08:00-16:30','','GB','https://www.tradinghours.com/exchanges/lse'),(30,'LN','Euronext London','XLDN','','','','',''),(31,'LS','NYSE EURONEXT - EURONEXT LISBON','XLIS','Europe/Lisbon','09:00-17:30','','','https://www.tradinghours.com/exchanges/euronext-lisbon'),(32,'MC','BOLSA DE MADRID','XMAD','Europe/Madrid','09:00-17:30','','','https://www.tradinghours.com/exchanges/bme'),(33,'ME','MOSCOW EXCHANGE','MISX','Europe/Moscow','09:30-19:00','','','https://www.tradinghours.com/exchanges/moex'),(34,'MI','Italian Stock Exchange','XMIL','Europe/Rome','08:00-17:42','','','https://www.tradinghours.com/exchanges/mta'),(35,'MU','BOERSE MUENCHEN','XMUN','Europe/Berlin','08:00-20:00','','','https://www.tradinghours.com/exchanges/xmun'),(36,'MX','BOLSA MEXICANA DE VALORES (MEXICAN STOCK EXCHANGE)','XMEX','America/Mexico_City','08:00-15:10','','','https://www.tradinghours.com/exchanges/bmv'),(37,'NE','AEQUITAS NEO EXCHANGE','NEOE','America/Toronto','09:30-16:00','','','https://www.tradinghours.com/exchanges/neo'),(38,'NL','Nigerian Stock Exchange','XNSA','Africa/Lagos','09:30-14:30','','',''),(39,'NS','NATIONAL STOCK EXCHANGE OF INDIA','XNSE','Asia/Kolkata','09:00-16:00','','IN','https://www.tradinghours.com/exchanges/nse-india'),(40,'NZ','NEW ZEALAND EXCHANGE LTD','XNZE','Pacific/Auckland','10:00-16:45','','','https://www.tradinghours.com/exchanges/nzx'),(41,'OL','OSLO BORS ASA','XOSL','Europe/Oslo','08:15-17:30','','','https://www.tradinghours.com/exchanges/ose'),(42,'PA','NYSE EURONEXT - MARCHE LIBRE PARIS','XPAR','Europe/Paris','09:00-17:30','','','https://www.tradinghours.com/exchanges/euronext-paris'),(43,'PM','Philippine Stock Exchange','XPHS','Asia/Manila','09:30-13:00','','','https://www.tradinghours.com/markets/pse'),(44,'PR','PRAGUE STOCK EXCHANGE','XPRA','Europe/Prague','08:00-17:00','','','https://www.tradinghours.com/exchanges/xpra'),(45,'QA','QATAR EXCHANGE','DSMD','Asia/Qatar','09:00-13:15','','','https://www.tradinghours.com/exchanges/qe'),(46,'RG','NASDAQ OMX RIGA','XRIS','Europe/Riga','09:00-16:30','','','https://www.tradinghours.com/exchanges/omxr-riga'),(47,'SA','Brazil Bolsa - Sao Paolo','BVMF','America/Sao_Paulo','09:45-18:45','','','https://www.tradinghours.com/exchanges/bovespa'),(48,'SG','BOERSE STUTTGART','XSTU','Asia/Amman','08:00-20:00','','','https://www.tradinghours.com/exchanges/xstu'),(49,'SI','SINGAPORE EXCHANGE','XSES','Asia/Singapore','08:30-17:16','','','https://www.tradinghours.com/exchanges/sgx'),(50,'SN','SANTIAGO STOCK EXCHANGE','XSGO','America/Santiago','09:30-16:00','','','https://www.tradinghours.com/exchanges/bvs'),(51,'SR','SAUDI STOCK EXCHANGE','XSAU','Asia/Riyadh','10:00-15:10','6,7','SA','https://www.tradinghours.com/exchanges/tadawul'),(52,'SS','SHANGHAI STOCK EXCHANGE','XSHG','Asia/Brunei','09:15-15:30','','','https://www.tradinghours.com/exchanges/sse'),(53,'ST','NASDAQ OMX NORDIC STOCKHOLM','XSTO','Europe/Stockholm','08:00-18:00','','','https://www.tradinghours.com/exchanges/xngm'),(54,'SW','SWISS EXCHANGE','XSWX','Europe/Zurich','09:30-17:00','','','https://www.tradinghours.com/exchanges/six'),(55,'SZ','SHENZHEN STOCK EXCHANGE','XSHE','Asia/Shanghai','09:15-15:00','','','https://www.tradinghours.com/exchanges/szse'),(56,'T','TOKYO STOCK EXCHANGE-TOKYO PRO MARKET','XJPX','Asia/Tokyo','09:00-15:00','','','https://www.tradinghours.com/exchanges/jpx'),(57,'TA','TEL AVIV STOCK EXCHANGE','XTAE','Asia/Jerusalem','09:45-17:14','6,7','','https://www.tradinghours.com/exchanges/tase'),(58,'TL','NASDAQ OMX TALLINN','XTAL','Europe/Tallinn','09:00-16:30','','','https://www.tradinghours.com/exchanges/omxt-tallinn'),(59,'TO','TORONTO STOCK EXCHANGE','XTSE','America/Toronto','09:30-16:00','','','https://www.tradinghours.com/exchanges/tsx'),(60,'TW','TAIWAN STOCK EXCHANGE','XTAI','Asia/Taipei','09:00-17:00','','TW','https://www.tradinghours.com/exchanges/twse'),(61,'US','US exchanges (NYSE, Nasdaq)','XNYS,XASE,BATS,ARCX,XNMS,XNCM,XNGS,IEXG','America/New_York','09:30-16:00','','','https://www.tradinghours.com/exchanges/nyse'),(62,'V','TSX VENTURE EXCHANGE - NEX','XTSX','America/Toronto','09:30-16:15','','','https://www.tradinghours.com/exchanges/xtsx'),(63,'VI','Vienna Stock Exchange','XWBO','Europe/Vienna','09:04-17:30','','','https://www.tradinghours.com/exchanges/vse'),(64,'VN','Vietnam exchanges including HOSE, HNX and UPCOM','HSTC, XSTC','Asia/Bangkok','09:00-15:00','','','https://www.tradinghours.com/exchanges/hose'),(65,'VS','NASDAQ OMX VILNIUS','XLIT','Europe/Vilnius','09:00-16:30','','','https://www.tradinghours.com/exchanges/omxv-vilnius'),(66,'WA','WARSAW STOCK EXCHANGE/EQUITIES/MAIN MARKET','XWAR','Europe/Warsaw','08:30-17:05','','','https://www.tradinghours.com/exchanges/gpw'),(67,'HA','Hanover Stock Exchange','XHAN','Europe/Berlin','','','',''),(68,'SX','DEUTSCHE BOERSE Stoxx','','Europe/Berlin','','','',''),(69,'TG ','DEUTSCHE BOERSE TradeGate ','','Europe/Berlin','','','',''),(70,'SC','BOERSE_FRANKFURT_ZERTIFIKATE','','Europe/Berlin','','','','');
/*!40000 ALTER TABLE `finnhub_stock_exchange` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-01-03  5:35:11
