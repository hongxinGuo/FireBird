///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ChinaMarket.h"

#include"SystemConfiguration.h"

#include "NeteaseRTDataSource.h"
#include "SinaRTDataSource.h"

#include<string>
#include<fstream>

#include "resource.h"
#include "TengxunRTDataSource.h"

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
	"NeteaseRTDataInquiryPerTime" : 900,
	"TengxunRTDataInquiryPerTime" : 900
},

"WorldMarket" : {
	"MarketResettingTime" : 170000,
	"FinnhubToken" : "bv985d748v6ujthqfke0",
	"FinnhubAccountFeePaid" : true,
	"TiingoToken" : "c897a00b7cfc2adffc630d23befd5316a4683156",
	"TiingoAccountFeePaid" : false,
	"QuandlToken" : "aBMXMyoTyiy_N3pMb3ex",
	"QuandlAccountFeePaid" : false,
	"FinnhubInquiryTime" : 1100,
	"TiingoInquiryTime" : 9000,
	"QuandlInquiryTime" : 36000
},

"FinancialDataUpdateRate" : {
	"StockProfile" : 365,
	"BasicFinancial" : 45,
	"InsideTransaction" : 30,
	"InsideSentiment" : 30,
	"StockPeer" : 90
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

	m_fUpdate = false; // update flag
	m_fInitialized = true;
	char buffer[200];
	_getcwd(buffer, 200);
	m_strDirectory = buffer;
	m_strDirectory = m_strDirectory + _T("\\"); //
	m_strFileName = _T("SystemConfiguration.json"); // json file name

	// 系统配置 
	m_bDebugMode = false;
	m_bReloadSystem = false;
	m_strDatabaseAccountName = _T("FireBird");
	m_strDatabaseAccountPassword = _T("firebird");
	m_iBackgroundThreadPermittedNumber = 8; // 后台线程最多8个

	// China Market
	m_iChinaMarketRealtimeServer = 0; // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据。
	m_iChinaMarketDayLineServer = 0; // 日线数据服务器。0:网易日线服务器；1：腾讯日线服务器
	m_iChinaMarketRTDataInquiryTime = 250; // 默认实时数据查询时间间隔为250毫秒
	m_iSavingChinaMarketStockDayLineThread = 4; // 默认中国股票历史数据存储线程数为4
#ifdef DEBUG
	m_bFastInquiringRTData = false; // 用于测试。当需要测试系统实时数据接收负载时，DEBUG状态时设置为真。默认为假
#else
	m_bFastInquiringRTData = false;
#endif
	m_iNumberOfRTDataSource = 4;
	m_iSinaRTDataInquiryPerTime = 850;
	m_iNeteaseRTDataInquiryPerTime = 900;
	m_iTengxunRTDataInquiryPerTime = 900;

	// World Market
	m_lMarketResettingTime = 170000; // 默认市场重置时间为170000
	m_strFinnhubToken = "bv985d748v6ujthqfke0"; // Finnhub token
	m_strTiingoToken = "c897a00b7cfc2adffc630d23befd5316a4683156"; // Tiingo token
	m_strQuandlToken = _T(""); // Quandl token
	m_iWorldMarketFinnhubInquiryTime = 1100; // 默认每小时最多查询3300次
	m_iWorldMarketTiingoInquiryTime = 3600000 / 500; // 默认每小时最多查询500次。 默认免费账户的查询频率为每小时500次(每次7200毫秒）；付费账户为每小时20000次（每次180毫秒）
	m_iWorldMarketQuandlInquiryTime = 3600000 / 100; // 默认每小时最多查询100次
	m_bFinnhubAccountFeePaid = true; // 默认为付费账户，由程序决定是否改为免费账户（自己改也可以）
	m_bQuandlAccountFeePaid = true;// 默认为付费账户，由程序决定是否改为免费账户（自己改也可以）
	m_bQuandlAccountFeePaid = true;// 默认为付费账户，由程序决定是否改为免费账户（自己改也可以）

	// WebSocket
	m_bUsingFinnhubWebSocket = true; // 是否使用Finnhub的WebSocket
	m_bUsingTiingoIEXWebSocket = true; // 是否使用Tiingo的WebSocket
	m_bUsingTiingoCryptoWebSocket = true; // 是否使用Tiingo的WebSocket
	m_bUsingTiingoForexWebSocket = true; // 是否使用Tiingo的WebSocket
	m_bUsingQuandlWebSocket = true;

	// Data Update Rate
	m_iStockProfileUpdateRate = 365; // 股票概况更新频率，单位为天。默认为365天。
	m_iStockBasicFinancialUpdateRate = 45; // 基本财务更新频率，单位为天。默认为45天。
	m_iInsideTransactionUpdateRate = 30; // 内部交易更新频率，单位为天。默认为30天。
	m_iInsideSentimentUpdateRate = 30; // 内部交易情绪更新频率，单位为天。默认为30天。
	m_iStockPeerUpdateRate = 90; // 股票对手更新频率，单位为天。默认为90天。
	m_iEPSSurpriseUpdateRate = 90;
	m_iSECFilingsUpdateRate = 30;

	// spdlog日志等级
	m_iLogLevel = SPDLOG_LEVEL_TRACE; // 默认记录等级为跟踪级（所有日志皆记录）

	// 测试系统选项
	m_strBenchmarkTestFileDirectory = _T("C:\\FireBird\\Test Data\\Benchmark\\"); // Benchmark默认目录

	if (!LoadDB()) {
		m_fUpdate = true;
	}

	// 具体工作计算机的初始参数
	m_rSystemDisplay.left = 0;
	m_rSystemDisplay.right = 2600;
	m_rSystemDisplay.top = 0;
	m_rSystemDisplay.bottom = 1600;
	m_rCurrentWindow.left = 0;
	m_rCurrentWindow.right = 2600;
	m_rCurrentWindow.top = 0;
	m_rCurrentWindow.bottom = 1600;
}

CSystemConfiguration::~CSystemConfiguration() {
	if (IsNeedUpdate()) {
		UpdateDB();
	}
}

void CSystemConfiguration::UpdateDB() {
	const CString strOld = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("json");
	const CString strNew = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("bak");
	DeleteFile(GetConfigurationFileDirectory() + strNew);
	rename(GetConfigurationFileDirectory() + strOld, GetConfigurationFileDirectory() + strNew); // 保存备份
	SaveDB();
	SetUpdate(false);
}

void CSystemConfiguration::Update(json& jsonData) {
	string sTemp;
	CString str1, str2, str3;
	str1.LoadString(nullptr, IDS_PROPERTYVIEW_SYSTEM_STATUS); // Note 使用LoadString时，必须明确声明hInstance为nullptr，否则报错
	str2.LoadString(nullptr, IDS_PROPERTYVIEW_PROPERTIES_WINDOW);
	str3.LoadString(nullptr, IDS_PROPERTYVIEW_PROPERTIES2);
	// 环境配置
	try {
		sTemp = jsonData.at("Environment").at("Display").at("PropertyPage");
		if (sTemp.c_str() == str1) {
			m_iDisplayPropertyPage = 0;
		}
		else if (sTemp.c_str() == str2) {
			m_iDisplayPropertyPage = 1;
		}
		else if (sTemp.c_str() == str3) {
			m_iDisplayPropertyPage = 2;
		}
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}

	// 系统配置
	try {
		m_iLogLevel = jsonData.at("SystemConfiguration").at("LogLevel");
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}

	try {
		m_bDebugMode = jsonData.at("SystemConfiguration").at("DebugMode");
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_bReloadSystem = jsonData.at("SystemConfiguration").at("ReloadSystem");
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}

	try {
		sTemp = jsonData.at("SystemConfiguration").at("DatabaseAccountName");
		m_strDatabaseAccountName = sTemp.c_str();
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		sTemp = jsonData.at("SystemConfiguration").at("DatabaseAccountPassword");
		m_strDatabaseAccountPassword = sTemp.c_str();
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iBackgroundThreadPermittedNumber = jsonData.at("SystemConfiguration").at("BackgroundThreadPermittedNumber");
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}

	// ChinaMarket
	try {
		sTemp = jsonData.at("ChinaMarket").at("RealtimeServer"); // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据（目前不使用）。
		if (sTemp == _T("sina")) {
			m_iChinaMarketRealtimeServer = 0;
		}
		else if (sTemp == _T("netease")) {
			m_iChinaMarketRealtimeServer = 1;
		}
		else if (sTemp == _T("tengxun")) {
			m_iChinaMarketRealtimeServer = 2;
		}
		else { // 非法服务器名称，使用默认sina服务器
			m_iChinaMarketRealtimeServer = 0;
			m_fUpdate = true;
		}
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		sTemp = jsonData.at("ChinaMarket").at("DayLineServer"); // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据（目前不使用）。
		if (sTemp == _T("netease")) {
			m_iChinaMarketDayLineServer = 0;
		}
		else if (sTemp == _T("tengxun")) {
			m_iChinaMarketDayLineServer = 1;
		}
		else {// 非法服务器名称，使用默认sina服务器
			m_iChinaMarketDayLineServer = 0;
			m_fUpdate = true;
		}
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iChinaMarketRTDataInquiryTime = jsonData.at("ChinaMarket").at("RealtimeInquiryTime"); // 实时数据查询时间间隔（单位：毫秒）
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iSavingChinaMarketStockDayLineThread = jsonData.at("ChinaMarket").at("SavingStockDayLineThread"); // 保存股票日线数据线程数量
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iNumberOfRTDataSource = jsonData.at("ChinaMarket").at("NumberOfRTDataSource"); // Sina实时数据申请引擎数
		if (m_iNumberOfRTDataSource > 4) {
			m_iNumberOfRTDataSource = 4;
			m_fUpdate = true;
		}
		if (m_iNumberOfRTDataSource < 1) {
			m_iNumberOfRTDataSource = 1;
			m_fUpdate = true;
		}
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iSinaRTDataInquiryPerTime = jsonData.at("ChinaMarket").at("SinaRTDataInquiryPerTime"); // Sina实时数据每次查询股票数
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iNeteaseRTDataInquiryPerTime = jsonData.at("ChinaMarket").at("NeteaseRTDataInquiryPerTime"); // Sina实时数据每次查询股票数
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iTengxunRTDataInquiryPerTime = jsonData.at("ChinaMarket").at("TengxunRTDataInquiryPerTime"); // Sina实时数据每次查询股票数
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}

	// WorldMarket
	try {
		m_lMarketResettingTime = jsonData.at("WorldMarket").at("MarketResettingTime"); // 市场重置时间
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		sTemp = jsonData.at("WorldMarket").at("FinnhubToken"); // Finnhub token
		m_strFinnhubToken = sTemp.c_str();
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_bFinnhubAccountFeePaid = jsonData.at("WorldMarket").at("FinnhubAccountFeePaid");
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		sTemp = jsonData.at("WorldMarket").at("TiingoToken"); // Tiingo token
		m_strTiingoToken = sTemp.c_str();
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_bTiingoAccountFeePaid = jsonData.at("WorldMarket").at("TiingoAccountFeePaid");
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		sTemp = jsonData.at("WorldMarket").at("QuandlToken"); // Quandl token
		m_strQuandlToken = sTemp.c_str();
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_bQuandlAccountFeePaid = jsonData.at("WorldMarket").at("QuandlAccountFeePaid");
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iWorldMarketFinnhubInquiryTime = jsonData.at("WorldMarket").at("FinnhubInquiryTime"); // 默认每小时最多查询3000次
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iWorldMarketTiingoInquiryTime = jsonData.at("WorldMarket").at("TiingoInquiryTime"); // 默认每小时最多查询400次
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iWorldMarketQuandlInquiryTime = jsonData.at("WorldMarket").at("QuandlInquiryTime"); // 默认每小时最多查询100次
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}

	// WebSocket
	try {
		m_bUsingFinnhubWebSocket = jsonData.at("WebSocket").at("UsingFinnhubWebSocket"); // 是否使用Finnhub的WebSocket
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_bUsingTiingoIEXWebSocket = jsonData.at("WebSocket").at("UsingTiingoIEXWebSocket"); // 是否使用Tiingo的WebSocket
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_bUsingTiingoCryptoWebSocket = jsonData.at("WebSocket").at("UsingTiingoCryptoWebSocket"); // 是否使用Tiingo的WebSocket
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_bUsingTiingoForexWebSocket = jsonData.at("WebSocket").at("UsingTiingoForexWebSocket"); // 是否使用Tiingo的WebSocket
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}

	// Financial Data Update Rate
	try {
		m_iStockBasicFinancialUpdateRate = jsonData.at("FinancialDataUpdateRate").at("StockBasicFinancial");
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iStockProfileUpdateRate = jsonData.at("FinancialDataUpdateRate").at("StockProfile");
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iInsideTransactionUpdateRate = jsonData.at("FinancialDataUpdateRate").at("InsideTransaction");
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iInsideSentimentUpdateRate = jsonData.at("FinancialDataUpdateRate").at("InsideSentiment");
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iStockPeerUpdateRate = jsonData.at("FinancialDataUpdateRate").at("StockPeer");
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}

	try {
		m_iEPSSurpriseUpdateRate = jsonData.at("FinancialDataUpdateRate").at("EPSSurprise");
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iSECFilingsUpdateRate = jsonData.at("FinancialDataUpdateRate").at("SECFilings");
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}

	// 测试系统选项
	try {
		sTemp = jsonData.at("TestConfiguration").at("BenchmarkTestFileDirectory");
		m_strBenchmarkTestFileDirectory = sTemp.c_str();
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
}

void CSystemConfiguration::UpdateJsonData(json& jsonData) {
	jsonData.clear(); // 清除之前的数据。
	CString str;

	// Environment
	switch (m_iDisplayPropertyPage) {
	case 0:
		str.LoadString(nullptr, IDS_PROPERTYVIEW_SYSTEM_STATUS);
		jsonData["Environment"]["Display"]["PropertyPage"] = str;
		break;
	case 1:
		str.LoadString(nullptr, IDS_PROPERTYVIEW_PROPERTIES_WINDOW);
		break;
	case 2:
		str.LoadString(nullptr, IDS_PROPERTYVIEW_PROPERTIES2);
		break;
	default:
		str.LoadString(nullptr, IDS_PROPERTYVIEW_SYSTEM_STATUS);
		break;
	}
	jsonData["Environment"]["Display"]["PropertyPage"] = str;

	// system
	jsonData["SystemConfiguration"]["LogLevel"] = m_iLogLevel;
	jsonData["SystemConfiguration"]["DebugMode"] = m_bDebugMode;
	jsonData["SystemConfiguration"]["ReloadSystem"] = m_bReloadSystem;
	jsonData["SystemConfiguration"]["DatabaseAccountName"] = m_strDatabaseAccountName;
	jsonData["SystemConfiguration"]["DatabaseAccountPassword"] = m_strDatabaseAccountPassword;
	jsonData["SystemConfiguration"]["BackgroundThreadPermittedNumber"] = m_iBackgroundThreadPermittedNumber;

	// China market
	switch (m_iChinaMarketRealtimeServer) {
	case 0:
		jsonData["ChinaMarket"]["RealtimeServer"] = _T("sina");
		break;
	case 1:
		jsonData["ChinaMarket"]["RealtimeServer"] = _T("netease");
		break;
	case 2:
		jsonData["ChinaMarket"]["RealtimeServer"] = _T("tengxun");
		break;
	default:
		jsonData["ChinaMarket"]["RealtimeServer"] = _T("sina");
		break;
	}
	switch (m_iChinaMarketDayLineServer) {
	case 0:
		jsonData["ChinaMarket"]["DayLineServer"] = _T("netease");
		break;
	case 1:
		jsonData["ChinaMarket"]["DayLineServer"] = _T("tengxun");
		break;
	default:
		jsonData["ChinaMarket"]["DayLineServer"] = _T("netease");
		break;
	}
	jsonData["ChinaMarket"]["NumberOfRTDataSource"] = m_iNumberOfRTDataSource;
	jsonData["ChinaMarket"]["RealtimeInquiryTime"] = m_iChinaMarketRTDataInquiryTime;
	jsonData["ChinaMarket"]["SavingStockDayLineThread"] = m_iSavingChinaMarketStockDayLineThread;
	jsonData["ChinaMarket"]["SinaRTDataInquiryPerTime"] = m_iSinaRTDataInquiryPerTime;
	jsonData["ChinaMarket"]["NeteaseRTDataInquiryPerTime"] = m_iNeteaseRTDataInquiryPerTime;
	jsonData["ChinaMarket"]["TengxunRTDataInquiryPerTime"] = m_iTengxunRTDataInquiryPerTime;

	// World market
	jsonData["WorldMarket"]["MarketResettingTime"] = m_lMarketResettingTime;
	jsonData["WorldMarket"]["FinnhubToken"] = m_strFinnhubToken;
	jsonData["WorldMarket"]["FinnhubAccountFeePaid"] = m_bFinnhubAccountFeePaid;
	jsonData["WorldMarket"]["TiingoToken"] = m_strTiingoToken;
	jsonData["WorldMarket"]["TiingoAccountFeePaid"] = m_bTiingoAccountFeePaid;
	jsonData["WorldMarket"]["QuandlToken"] = m_strQuandlToken;
	jsonData["WorldMarket"]["QuandlAccountFeePaid"] = m_bQuandlAccountFeePaid;
	jsonData["WorldMarket"]["FinnhubInquiryTime"] = m_iWorldMarketFinnhubInquiryTime;
	jsonData["WorldMarket"]["TiingoInquiryTime"] = m_iWorldMarketTiingoInquiryTime;
	jsonData["WorldMarket"]["QuandlInquiryTime"] = m_iWorldMarketQuandlInquiryTime;

	// Web socket
	jsonData["WebSocket"]["UsingFinnhubWebSocket"] = m_bUsingFinnhubWebSocket;
	jsonData["WebSocket"]["UsingTiingoIEXWebSocket"] = m_bUsingTiingoIEXWebSocket;
	jsonData["WebSocket"]["UsingTiingoCryptoWebSocket"] = m_bUsingTiingoCryptoWebSocket;
	jsonData["WebSocket"]["UsingTiingoForexWebSocket"] = m_bUsingTiingoForexWebSocket;

	jsonData["FinancialDataUpdateRate"]["StockProfile"] = m_iStockProfileUpdateRate;
	jsonData["FinancialDataUpdateRate"]["StockBasicFinancial"] = m_iStockBasicFinancialUpdateRate;
	jsonData["FinancialDataUpdateRate"]["InsideTransaction"] = m_iInsideTransactionUpdateRate;
	jsonData["FinancialDataUpdateRate"]["InsideSentiment"] = m_iInsideSentimentUpdateRate;
	jsonData["FinancialDataUpdateRate"]["StockPeer"] = m_iStockPeerUpdateRate;
	jsonData["FinancialDataUpdateRate"]["EPSSurprise"] = m_iEPSSurpriseUpdateRate;
	jsonData["FinancialDataUpdateRate"]["SECFilings"] = m_iSECFilingsUpdateRate;

	// 测试系统选项
	jsonData["TestConfiguration"]["BenchmarkTestFileDirectory"] = m_strBenchmarkTestFileDirectory;
}

void CSystemConfiguration::ChangeFinnhubAccountTypeToFree() {
	m_bFinnhubAccountFeePaid = false;
	m_iWorldMarketFinnhubInquiryTime = 1100; // 每次1100毫秒
	m_fUpdate = true;
}

void CSystemConfiguration::ChangeFinnhubAccountTypeToPaid() {
	m_bFinnhubAccountFeePaid = true;
	m_iWorldMarketFinnhubInquiryTime = 220; // 每次220毫秒
	m_fUpdate = true;
}

void CSystemConfiguration::UsingSinaRealtimeServer() {
	gl_pNeteaseRTDataSource->Enable(false);
	gl_pSinaRTDataSource->Enable(true);
	gl_pTengxunRTDataSource->Enable(false);
}

void CSystemConfiguration::UsingNeteaseRealtimeServer() {
	gl_pSinaRTDataSource->Enable(false);
	gl_pNeteaseRTDataSource->Enable(true);
	gl_pTengxunRTDataSource->Enable(false);
}

void CSystemConfiguration::UsingTengxunRealtimeServer() {
	gl_pSinaRTDataSource->Enable(false);
	gl_pNeteaseRTDataSource->Enable(false);
	gl_pTengxunRTDataSource->Enable(true);
}

bool CSystemConfiguration::IsWebBusy() {
	return gl_pSinaRTDataSource->IsWebError() || gl_pNeteaseRTDataSource->IsWebError();
}

bool CSystemConfiguration::LoadDB() {
	fstream f(GetConfigurationFileDirectoryAndName(), ios::in);
	if (f.is_open()) {
		json systemConfiguration;
		f >> systemConfiguration;
		//systemConfiguration = json::parse(f); // 这种方式等价于 f >> m_systemConfiguration;
		f.close();
		Update(systemConfiguration);
		return true;
	}
	return false;
}

void CSystemConfiguration::SaveDB() {
	json systemConfiguration;

	UpdateJsonData(systemConfiguration);
	fstream f(GetConfigurationFileDirectoryAndName(), ios::out);
	f << systemConfiguration;
	f.close();
}
