///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
module;

module SystemConfiguration;

import NlohmannJsonDeclaration; // 按照顺序输出json，必须使用此ordered_json,以保证解析后的数据与解析前的顺序一致。
import DataSource.SinaRT;
import DataSource.TengxunRT;
import TimeConvert;
#include <afx.h>
#include <direct.h>

using std::fstream;

import std;
using std::literals::chrono_literals::operator""ms;

bool CSystemConfiguration::sm_bInitialized = false;

/// <summary>
/// 系统配置文件，采用nlohmann json库格式。
///
/// </summary>
std::string gl_sSystemConfiguration = R"(
{
"Environment": {
	"Display": {
		"PropertyPage" : "System Status"
	}
},

"SystemConfiguration": {
	"LogLevel" : 2,
	"UsingFastCPU" : true,
	"DebugMode" : false,
	"ReloadSystem" : false,
  "DatabaseAccountName" : "FireBird",
	"DatabaseAccountPassword" : "firebird",
	"BackgroundThreadPermittedNumber" : 8,
	"SavingThreadPermittedNumber" : 4
},

"WebSocket" : {
	"UsingFinnhubWebSocket" : true,
	"UsingTiingoIEXWebSocket" : true,
	"UsingTiingoCryptoWebSocket" : true,
	"UsingTiingoForexWebSocket" : true
},

"ChinaMarket" : {
	"RealtimeServer" : "sina",
	"DayLineServer" : "tengxun",
	"RealtimeInquiryTime" : 250,
	"SavingStockDayLineThread" : 4,
	"FastInquiringRealtimeData" : false,
	"NumberOfRTDataSource" : 4,
	"SinaRTDataInquiryPerTime" : 850,
	"TengxunRTDataInquiryPerTime" : 900,
	"CurrentStock" : "600026.SS"
},

"WorldMarket" : {
	"MarketResettingTime" : 170000,
	"FinnhubToken" : "bv985d748v6u0",
	"FinnhubAccountFeePaid" : true,
	"FinnhubInquiryTime" : 1100,
	"TiingoInquiryTime" : 9000
},

"Tiingo" : {
	"IEXTopOfBookUpdateDate" : 19990101,
	"StockDayLineProcessedDate" : 19900101,
	"Stock52WeekHighLowUpdateDate" : 19820101,
	"Token" : "c897a00b7cfc2630d235316a4683156",
	"HourlyRequestLimit" : 500,
	"DailyRequestLimit" : 20000,
	"BandWidth" : 5368709120,
	"BandWidthLeft" : 5368709120
},

"FinancialDataUpdateRate" : {
	"StockProfile" : 365,
	"BasicFinancial" : 45,
	"InsideTransaction" : 30,
	"InsideSentiment" : 30,
	"StockPeer" : 90,
	"TiingoCompanyFinancialState" : 30
},
"TestConfiguration" : {
	"BenchmarkTestFileDirectory" : "C:\\FireBird\\Test Data\\Benchmark\\"
}
})";

// 确保SystemConfiguration是第一个初始化的全局变量。因其他全局变量可能会使用该变量的内容。
CSystemConfiguration::CSystemConfiguration() {
	ASSERT(!sm_bInitialized); // 只生成唯一实例
	if (sm_bInitialized) {
		TRACE(_T("GlobeOption全局变量只允许存在一个实例\n"));
#ifdef _DEBUG
		ASSERT(FALSE);
#endif // _DEBUG
	}
	sm_bInitialized = true;

	char buffer[200];
	_getcwd(buffer, 200);
	m_strDirectory = buffer;
	m_strDirectory = m_strDirectory + "\\"; //
	m_strFileName = "SystemConfiguration.json"; // json file name

#ifdef DEBUG
	m_bFastInquiringRTData = false; // 用于测试。当需要测试系统实时数据接收负载时，DEBUG状态时设置为真。默认为假
#else
	m_bFastInquiringRTData = false;
#endif

	if (!LoadDB()) {
		m_fUpdateDB = true;
	}
}

CSystemConfiguration::~CSystemConfiguration() {
	if (IsUpdateDB()) {
		UpdateDB();
	}
}

void CSystemConfiguration::UpdateDB() {
	const string strOld = m_strFileName.substr(0, m_strFileName.length() - 4) + "json";
	const string strNew = m_strFileName.substr(0, m_strFileName.length() - 4) + "bak";
	std::filesystem::remove(GetConfigurationFileDirectory() + strNew);
	rename((GetConfigurationFileDirectory() + strOld).c_str(), (GetConfigurationFileDirectory() + strNew).c_str()); // 保存备份

	SaveDB();
	SetUpdateDB(false);
}

void CSystemConfiguration::Update(shared_ptr<nlohmannJson> pJsonData) {
	string sTemp;

	// 系统配置
	try {
		m_iLogLevel = pJsonData->at("SystemConfiguration").at("LogLevel");
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}

	try {
		m_bDebugMode = pJsonData->at("SystemConfiguration").at("DebugMode");
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_bReloadSystem = pJsonData->at("SystemConfiguration").at("ReloadSystem");
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}

	try {
		sTemp = pJsonData->at("SystemConfiguration").at("DatabaseAccountName");
		m_strDatabaseAccountName = sTemp;
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		sTemp = pJsonData->at("SystemConfiguration").at("DatabaseAccountPassword");
		m_strDatabaseAccountPassword = sTemp;
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iBackgroundThreadPermittedNumber = pJsonData->at("SystemConfiguration").at("BackgroundThreadPermittedNumber");
		m_iBackgroundThreadPermittedNumber = min(m_iBackgroundThreadPermittedNumber, 16);
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}

	// ChinaMarket
	try {
		sTemp = pJsonData->at("ChinaMarket").at("RealtimeServer"); // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据（目前不使用）。
		if (sTemp == "sina") {
			m_iChinaMarketRealtimeServer = SinaRealTime_;
		}
		else if (sTemp == "netease") {
			m_iChinaMarketRealtimeServer = SinaRealTime_; //Note 网易实时数据服务器目前不可用，使用新浪服务器代替
			m_fUpdateDB = true;
		}
		else if (sTemp == "tengxun") {
			m_iChinaMarketRealtimeServer = TengxunRealTime_;
		}
		else { // 非法服务器名称，使用默认sina服务器
			m_iChinaMarketRealtimeServer = TengxunRealTime_;
			m_fUpdateDB = true;
		}
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		sTemp = pJsonData->at("ChinaMarket").at("DayLineServer"); // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据（目前不使用）。
		if (sTemp == "netease") {
			m_iChinaMarketDayLineServer = EastmoneyDayLine_; //Note 网易日线数据服务器目前不可用，使用东方财富服务器代替
			m_fUpdateDB = true;
		}
		else if (sTemp == "tengxun") {
			m_iChinaMarketDayLineServer = TengxunDayLine_;
		}
		else if (sTemp == "eastmoney") {
			m_iChinaMarketDayLineServer = EastmoneyDayLine_;
		}
		else {// 非法服务器名称，使用默认东方财富服务器
			m_iChinaMarketDayLineServer = EastmoneyDayLine_;
			m_fUpdateDB = true;
		}
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		//m_iChinaMarketRTDataInquiryTime = jsonData->at("ChinaMarket").at("RealtimeInquiryTime"); // 实时数据查询时间间隔（单位：毫秒）
		int duration = pJsonData->at("ChinaMarket").at("RealtimeInquiryTime");
		m_chinaMarketRTDataInquiryTime = milliseconds(duration); // 实时数据查询时间间隔（单位：毫秒）
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iSavingChinaMarketStockDayLineThread = pJsonData->at("ChinaMarket").at("SavingStockDayLineThread"); // 保存股票日线数据线程数量
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iNumberOfRTDataSource = pJsonData->at("ChinaMarket").at("NumberOfRTDataSource"); // Sina实时数据申请引擎数
		if (m_iNumberOfRTDataSource > 4) {
			m_iNumberOfRTDataSource = 4;
			m_fUpdateDB = true;
		}
		if (m_iNumberOfRTDataSource < 1) {
			m_iNumberOfRTDataSource = 1;
			m_fUpdateDB = true;
		}
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iSinaRTDataInquiryPerTime = pJsonData->at("ChinaMarket").at("SinaRTDataInquiryPerTime"); // Sina实时数据每次查询股票数
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iTengxunRTDataInquiryPerTime = pJsonData->at("ChinaMarket").at("TengxunRTDataInquiryPerTime"); // Sina实时数据每次查询股票数
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		sTemp = pJsonData->at("ChinaMarket").at("CurrentStock"); // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据（目前不使用）。
		m_strCurrentStock = sTemp;
	} catch (nlohmannJson::out_of_range&) {
		m_strCurrentStock = "";
		m_fUpdateDB = true;
	}

	// WorldMarket
	try {
		m_lMarketResettingTime = pJsonData->at("WorldMarket").at("MarketResettingTime"); // 市场重置时间
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		sTemp = pJsonData->at("WorldMarket").at("FinnhubToken"); // Finnhub token
		m_strFinnhubToken = sTemp;
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		int duration = pJsonData->at("WorldMarket").at("FinnhubInquiryTime");
		m_worldMarketFinnhubInquiryTime = milliseconds(duration); // 默认每小时最多查询3000次
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		int duration = pJsonData->at("WorldMarket").at("TiingoInquiryTime");
		m_worldMarketTiingoInquiryTime = milliseconds(duration); // 默认每小时最多查询400次
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}

	// Tiingo.com
	try {
		m_bTiingoAccountFeePaid = pJsonData->at("Tiingo").at("AccountFeePaid"); // 
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		sTemp = pJsonData->at("Tiingo").at("Token"); //
		m_strTiingoToken = sTemp;
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_lTiingoFundamentalsMetaUpdateDate = toLocalDays(pJsonData->at("Tiingo").at("FundamentalsMetaUpdateDate")); // 
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_lTiingoCryptoSymbolUpdateDate = toLocalDays(pJsonData->at("Tiingo").at("CryptoSymbolUpdateDate")); // 
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_lTiingoIEXTopOfBookUpdateDate = toLocalDays(pJsonData->at("Tiingo").at("IEXTopOfBookUpdateDate")); // 
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_lTiingoStockDayLineProcessedDate = toLocalDays(pJsonData->at("Tiingo").at("StockDayLineProcessedDate")); // 
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_lTiingoStock52WeekHighLowUpdateDate = toLocalDays(pJsonData->at("Tiingo").at("Stock52WeekHighLowUpdateDate")); // 
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iTiingoHourLyRequestLimit = pJsonData->at("Tiingo").at("HourlyRequestLimit"); // 
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_lTiingoDailyRequestLimit = pJsonData->at("Tiingo").at("DailyRequestLimit"); // 
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_llTiingoBandWidth = pJsonData->at("Tiingo").at("BandWidth"); // 
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_llTiingoBandWidthLeft = pJsonData->at("Tiingo").at("BandWidthLeft"); // 
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}

	try {
		sTemp = pJsonData->at("AlphaVantage").at("Token"); //
		m_strAlphaVantageToken = sTemp;
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}

	// WebSocket
	try {
		m_bUsingFinnhubWebSocket = pJsonData->at("WebSocket").at("UsingFinnhubWebSocket"); // 是否使用Finnhub的WebSocket
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_bUsingTiingoIEXWebSocket = pJsonData->at("WebSocket").at("UsingTiingoIEXWebSocket"); // 是否使用Tiingo的WebSocket
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_bUsingTiingoCryptoWebSocket = pJsonData->at("WebSocket").at("UsingTiingoCryptoWebSocket"); // 是否使用Tiingo的WebSocket
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_bUsingTiingoForexWebSocket = pJsonData->at("WebSocket").at("UsingTiingoForexWebSocket"); // 是否使用Tiingo的WebSocket
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}

	// Financial Data Update Rate
	try {
		m_iStockBasicFinancialUpdateRate = pJsonData->at("FinancialDataUpdateRate").at("StockBasicFinancial");
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iStockProfileUpdateRate = pJsonData->at("FinancialDataUpdateRate").at("StockProfile");
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iInsideTransactionUpdateRate = pJsonData->at("FinancialDataUpdateRate").at("InsideTransaction");
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iInsideSentimentUpdateRate = pJsonData->at("FinancialDataUpdateRate").at("InsideSentiment");
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iStockPeerUpdateRate = pJsonData->at("FinancialDataUpdateRate").at("StockPeer");
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iTiingoCompanyFinancialStateUpdateRate = pJsonData->at("FinancialDataUpdateRate").at("TiingoCompanyFinancialState");
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}

	try {
		m_iEPSSurpriseUpdateRate = pJsonData->at("FinancialDataUpdateRate").at("EPSSurprise");
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iSECFilingsUpdateRate = pJsonData->at("FinancialDataUpdateRate").at("SECFilings");
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}

	// 测试系统选项
	try {
		sTemp = pJsonData->at("TestConfiguration").at("BenchmarkTestFileDirectory");
		m_strBenchmarkTestFileDirectory = sTemp;
	} catch (nlohmannJson::out_of_range&) {
		m_fUpdateDB = true;
	}
}

void CSystemConfiguration::UpdateJsonData(shared_ptr<nlohmannJson> pJsonData) {
	pJsonData->clear(); // 清除之前的数据。
	std::wstring ws;

	// system
	(*pJsonData)["SystemConfiguration"]["LogLevel"] = m_iLogLevel;
	(*pJsonData)["SystemConfiguration"]["DebugMode"] = m_bDebugMode;
	(*pJsonData)["SystemConfiguration"]["ReloadSystem"] = m_bReloadSystem;
	(*pJsonData)["SystemConfiguration"]["DatabaseAccountName"] = m_strDatabaseAccountName;
	(*pJsonData)["SystemConfiguration"]["DatabaseAccountPassword"] = m_strDatabaseAccountPassword;
	(*pJsonData)["SystemConfiguration"]["BackgroundThreadPermittedNumber"] = m_iBackgroundThreadPermittedNumber;

	// China market
	switch (m_iChinaMarketRealtimeServer) {
	case SinaRealTime_:
		(*pJsonData)["ChinaMarket"]["RealtimeServer"] = "sina";
		break;
	case TengxunRealTime_:
		(*pJsonData)["ChinaMarket"]["RealtimeServer"] = "tengxun";
		break;
	default:
		(*pJsonData)["ChinaMarket"]["RealtimeServer"] = "sina";
		break;
	}
	switch (m_iChinaMarketDayLineServer) {
	case TengxunDayLine_:
		(*pJsonData)["ChinaMarket"]["DayLineServer"] = "tengxun";
		break;
	case EastmoneyDayLine_:
		(*pJsonData)["ChinaMarket"]["DayLineServer"] = "eastmoney";
	default:
		(*pJsonData)["ChinaMarket"]["DayLineServer"] = "eastmoney";
		break;
	}
	(*pJsonData)["ChinaMarket"]["NumberOfRTDataSource"] = m_iNumberOfRTDataSource;
	(*pJsonData)["ChinaMarket"]["RealtimeInquiryTime"] = m_chinaMarketRTDataInquiryTime.count();
	(*pJsonData)["ChinaMarket"]["SavingStockDayLineThread"] = m_iSavingChinaMarketStockDayLineThread;
	(*pJsonData)["ChinaMarket"]["SinaRTDataInquiryPerTime"] = m_iSinaRTDataInquiryPerTime;
	(*pJsonData)["ChinaMarket"]["TengxunRTDataInquiryPerTime"] = m_iTengxunRTDataInquiryPerTime;
	(*pJsonData)["ChinaMarket"]["CurrentStock"] = m_strCurrentStock;

	// World market
	(*pJsonData)["WorldMarket"]["MarketResettingTime"] = m_lMarketResettingTime;
	(*pJsonData)["WorldMarket"]["FinnhubToken"] = m_strFinnhubToken;
	(*pJsonData)["WorldMarket"]["FinnhubAccountFeePaid"] = m_bFinnhubAccountFeePaid;
	(*pJsonData)["WorldMarket"]["FinnhubInquiryTime"] = m_worldMarketFinnhubInquiryTime.count();
	(*pJsonData)["WorldMarket"]["TiingoInquiryTime"] = m_worldMarketTiingoInquiryTime.count();

	// Tiingo.com
	(*pJsonData)["Tiingo"]["AccountFeePaid"] = m_bTiingoAccountFeePaid;
	(*pJsonData)["Tiingo"]["Token"] = m_strTiingoToken;
	(*pJsonData)["Tiingo"]["FundamentalsMetaUpdateDate"] = toFormattedDate(m_lTiingoFundamentalsMetaUpdateDate);
	(*pJsonData)["Tiingo"]["CryptoSymbolUpdateDate"] = toFormattedDate(m_lTiingoCryptoSymbolUpdateDate);
	(*pJsonData)["Tiingo"]["IEXTopOfBookUpdateDate"] = toFormattedDate(m_lTiingoIEXTopOfBookUpdateDate);
	(*pJsonData)["Tiingo"]["StockDayLineProcessedDate"] = toFormattedDate(m_lTiingoStockDayLineProcessedDate);
	(*pJsonData)["Tiingo"]["Stock52WeekHighLowUpdate"] = toFormattedDate(m_lTiingoStock52WeekHighLowUpdateDate);
	(*pJsonData)["Tiingo"]["HourlyRequestLimit"] = m_iTiingoHourLyRequestLimit;
	(*pJsonData)["Tiingo"]["DailyRequestLimit"] = m_lTiingoDailyRequestLimit;
	(*pJsonData)["Tiingo"]["BandWidth"] = m_llTiingoBandWidth;
	(*pJsonData)["Tiingo"]["BandWidthLeft"] = m_llTiingoBandWidthLeft;

	// AlphaVantage
	(*pJsonData)["AlphaVantage"]["Token"] = m_strAlphaVantageToken;

	// Web socket
	(*pJsonData)["WebSocket"]["UsingFinnhubWebSocket"] = m_bUsingFinnhubWebSocket;
	(*pJsonData)["WebSocket"]["UsingTiingoIEXWebSocket"] = m_bUsingTiingoIEXWebSocket;
	(*pJsonData)["WebSocket"]["UsingTiingoCryptoWebSocket"] = m_bUsingTiingoCryptoWebSocket;
	(*pJsonData)["WebSocket"]["UsingTiingoForexWebSocket"] = m_bUsingTiingoForexWebSocket;

	(*pJsonData)["FinancialDataUpdateRate"]["StockProfile"] = m_iStockProfileUpdateRate;
	(*pJsonData)["FinancialDataUpdateRate"]["StockBasicFinancial"] = m_iStockBasicFinancialUpdateRate;
	(*pJsonData)["FinancialDataUpdateRate"]["InsideTransaction"] = m_iInsideTransactionUpdateRate;
	(*pJsonData)["FinancialDataUpdateRate"]["InsideSentiment"] = m_iInsideSentimentUpdateRate;
	(*pJsonData)["FinancialDataUpdateRate"]["StockPeer"] = m_iStockPeerUpdateRate;
	(*pJsonData)["FinancialDataUpdateRate"]["EPSSurprise"] = m_iEPSSurpriseUpdateRate;
	(*pJsonData)["FinancialDataUpdateRate"]["SECFilings"] = m_iSECFilingsUpdateRate;

	(*pJsonData)["FinancialDataUpdateRate"]["TiingoCompanyFinancialState"] = m_iTiingoCompanyFinancialStateUpdateRate;

	// 测试系统选项
	(*pJsonData)["TestConfiguration"]["BenchmarkTestFileDirectory"] = m_strBenchmarkTestFileDirectory;
}

void CSystemConfiguration::ChangeFinnhubAccountTypeToFree() {
	m_bFinnhubAccountFeePaid = false;
	m_worldMarketFinnhubInquiryTime = 1100ms; // 每次1100毫秒
	m_fUpdateDB = true;
}

void CSystemConfiguration::ChangeFinnhubAccountTypeToPaid() {
	m_bFinnhubAccountFeePaid = true;
	m_worldMarketFinnhubInquiryTime = 220ms; // 每次220毫秒
	m_fUpdateDB = true;
}

void CSystemConfiguration::ChangeTiingoAccountTypeToFree() {
	m_bTiingoAccountFeePaid = false;
	m_worldMarketTiingoInquiryTime = 9000ms; // 每次9000毫秒
	m_fUpdateDB = true;
}

void CSystemConfiguration::ChangeTiingoAccountTypeToPaid() {
	m_bTiingoAccountFeePaid = true;
	m_worldMarketTiingoInquiryTime = 500ms; // 每次200毫秒.每小时最大限额为20000。Note 由于两台机器同时申请，故而暂设为500ms。
	m_fUpdateDB = true;
}

void CSystemConfiguration::UsingSinaRealtimeServer() {
	gl_pSinaRTDataSource->Enable(true);
	gl_pTengxunRTDataSource->Enable(false);
}

void CSystemConfiguration::UsingTengxunRealtimeServer() {
	gl_pSinaRTDataSource->Enable(false);
	gl_pTengxunRTDataSource->Enable(true);
}

bool CSystemConfiguration::IsWebBusy() {
	return gl_pSinaRTDataSource->IsWebError();
}

bool CSystemConfiguration::LoadDB() {
	fstream f(GetConfigurationFileDirectoryAndName(), std::ios::in);
	if (f.is_open()) {
		shared_ptr<nlohmannJson> systemConfiguration = std::make_shared<nlohmannJson>();
		f >> *systemConfiguration;
		//systemConfiguration = nlohmannJson::parse(f); // 这种方式等价于 f >> m_systemConfiguration;
		f.close();
		Update(systemConfiguration);
		return true;
	}
	return false;
}

void CSystemConfiguration::SaveDB() {
	shared_ptr<nlohmannJson> pSystemConfiguration = std::make_shared<nlohmannJson>();

	UpdateJsonData(pSystemConfiguration);
	fstream f(GetConfigurationFileDirectoryAndName(), std::ios::out);
	f << *pSystemConfiguration;
	f.close();
}
