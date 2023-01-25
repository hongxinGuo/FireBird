///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ChinaMarket.h"

#include"SystemConfiguration.h"

#include<string>
#include<fstream>
using std::fstream;
using std::ios;

/// <summary>
/// 系统配置文件，采用nlohmann json库格式。
///
/// </summary>
std::string gl_sSystemConfiguration = R"(
{
"SystemConfiguration": {
	"UsingFastCPU" : true,
	"DebugMode" : false,
  "DatabaseAccountName" : "hxguo",
	"DatabaseAccountPassword" : "hxguo",
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
	"RealtimeInquiryTime" : 250,
	"SavingStockDayLineThread" : 4,
	"FastInquiringRealtimeData" : false,
	"SinaRTDataInquiryPerTime" : 850,
	"NeteaseRTDataInquiryPerTime" : 900,
	"TengxunRTDataInquiryPerTime" : 900
},

"WorldMarket" : {
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

CSystemConfiguration::CSystemConfiguration() {
	ASSERT(gl_bStartInitializeGlobeVariable == false); // 确保SystemConfiguration是第一个初始化的全局变量。因其他全局变量可能会使用该变量的内容。
	gl_bStartInitializeGlobeVariable = true;
	if (static int siInstance = 0; ++siInstance > 1) {
		TRACE(_T("GlobeOption全局变量只允许存在一个实例\n"));
#ifdef _DEBUG
		ASSERT(FALSE);
#endif // _DEBUG
	}

	m_fUpdate = false; // update flag
	m_fInitialized = true;
	m_strDirectory = _T("C:\\FireBird\\");
	m_strFileName = _T("SystemConfiguration.json"); // json file name

	// 系统配置
	m_bUsingFastCPU = true; // 默认使用快速CPU
	m_bDebugMode = false;
	m_strDatabaseAccountName = _T("hxguo");
	m_strDatabaseAccountPassword = _T("hxguo");
	m_iBackgroundThreadPermittedNumber = 8; // 后台线程最多8个
	m_iSavingThreadPermittedNumber = 4; // 默认存储线程数为4

	// China Market
	m_iChinaMarketRealtimeServer = 0; // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据（目前不使用）。
	m_iChinaMarketRTDataInquiryTime = 250; // 默认实时数据查询时间间隔为250毫秒
	m_iSavingChinaMarketStockDayLineThread = 4; // 默认中国股票历史数据存储线程数为4
#ifdef DEBUG
	m_bFastInquiringRTData = false; // 主要用于测试。当需要测试系统实时数据接收负载时，DEBUG状态时设置为真。
#else
	m_bFastInquiringRTData = false;
#endif
	m_iSinaRTDataInquiryPerTime = 850;
	m_iNeteaseRTDataInquiryPerTime = 900;
	m_iTengxunRTDataInquiryPerTime = 900;

	// World Market
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

	// 测试系统选项
	m_strBenchmarkTestFileDirectory = _T("C:\\FireBird\\Test Data\\Benchmark\\"); // Benchmark默认目录

	ASSERT(GetDefaultFileDirectoryAndName().Compare(_T("C:\\FireBird\\SystemConfiguration.json")) == 0);
	if (LoadDB()) {
		Update();
	}
	else { m_fUpdate = true; }

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
	const CString strOld = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("json");
	const CString strNew = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("bak");

	if (m_fUpdate) {
		DeleteFile(GetDefaultFileDirectory() + strNew);
		rename(GetDefaultFileDirectory() + strOld, GetDefaultFileDirectory() + strNew); // 保存备份
	}
	UpdateJson();
	SaveDB();
}

void CSystemConfiguration::Update() {
	string sTemp;

	try {
		// 系统配置
		try { m_bUsingFastCPU = m_systemConfiguration.at("SystemConfiguration").at("UsingFastCPU"); }
		catch (json::out_of_range&) { m_fUpdate = true; }
		try { m_bDebugMode = m_systemConfiguration.at("SystemConfiguration").at("DebugMode"); }
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			sTemp = m_systemConfiguration.at("SystemConfiguration").at("DatabaseAccountName");
			m_strDatabaseAccountName = sTemp.c_str();
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			sTemp = m_systemConfiguration.at("SystemConfiguration").at("DatabaseAccountPassword");
			m_strDatabaseAccountPassword = sTemp.c_str();
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try { m_iBackgroundThreadPermittedNumber = m_systemConfiguration.at("SystemConfiguration").at("BackgroundThreadPermittedNumber"); }
		catch (json::out_of_range&) { m_fUpdate = true; }
		try { m_iSavingThreadPermittedNumber = m_systemConfiguration.at("SystemConfiguration").at("SavingThreadPermittedNumber"); }
		catch (json::out_of_range&) { m_fUpdate = true; }

		// ChinaMarket
		try {
			sTemp = m_systemConfiguration.at("ChinaMarket").at("RealtimeServer"); // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据（目前不使用）。
			if (sTemp == _T("sina")) { m_iChinaMarketRealtimeServer = 0; }
			else if (sTemp == _T("netease")) { m_iChinaMarketRealtimeServer = 1; }
			else {
				// 非法服务器名称，使用默认sina服务器
				m_iChinaMarketRealtimeServer = 0;
				m_fUpdate = true;
			}
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iChinaMarketRTDataInquiryTime = m_systemConfiguration.at("ChinaMarket").at("RealtimeInquiryTime"); // 实时数据查询时间间隔（单位：毫秒）
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iSavingChinaMarketStockDayLineThread = m_systemConfiguration.at("ChinaMarket").at("SavingStockDayLineThread"); // 保存股票日线数据线程数量
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iSinaRTDataInquiryPerTime = m_systemConfiguration.at("ChinaMarket").at("SinaRTDataInquiryPerTime"); // Sina实时数据每次查询股票数
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iNeteaseRTDataInquiryPerTime = m_systemConfiguration.at("ChinaMarket").at("NeteaseRTDataInquiryPerTime"); // Sina实时数据每次查询股票数
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iTengxunRTDataInquiryPerTime = m_systemConfiguration.at("ChinaMarket").at("TengxunRTDataInquiryPerTime"); // Sina实时数据每次查询股票数
		}
		catch (json::out_of_range&) { m_fUpdate = true; }

		// WorldMarket
		try {
			sTemp = m_systemConfiguration.at("WorldMarket").at("FinnhubToken"); // Finnhub token
			m_strFinnhubToken = sTemp.c_str();
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_bFinnhubAccountFeePaid = m_systemConfiguration.at("WorldMarket").at("FinnhubAccountFeePaid");
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			sTemp = m_systemConfiguration.at("WorldMarket").at("TiingoToken"); // Tiingo token
			m_strTiingoToken = sTemp.c_str();
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_bTiingoAccountFeePaid = m_systemConfiguration.at("WorldMarket").at("TiingoAccountFeePaid");
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			sTemp = m_systemConfiguration.at("WorldMarket").at("QuandlToken"); // Quandl token
			m_strQuandlToken = sTemp.c_str();
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_bQuandlAccountFeePaid = m_systemConfiguration.at("WorldMarket").at("QuandlAccountFeePaid");
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iWorldMarketFinnhubInquiryTime = m_systemConfiguration.at("WorldMarket").at("FinnhubInquiryTime"); // 默认每小时最多查询3000次
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iWorldMarketTiingoInquiryTime = m_systemConfiguration.at("WorldMarket").at("TiingoInquiryTime"); // 默认每小时最多查询400次
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iWorldMarketQuandlInquiryTime = m_systemConfiguration.at("WorldMarket").at("QuandlInquiryTime"); // 默认每小时最多查询100次
		}
		catch (json::out_of_range&) { m_fUpdate = true; }

		// WebSocket
		try {
			m_bUsingFinnhubWebSocket = m_systemConfiguration.at("WebSocket").at("UsingFinnhubWebSocket"); // 是否使用Finnhub的WebSocket
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_bUsingTiingoIEXWebSocket = m_systemConfiguration.at("WebSocket").at("UsingTiingoIEXWebSocket"); // 是否使用Tiingo的WebSocket
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_bUsingTiingoCryptoWebSocket = m_systemConfiguration.at("WebSocket").at("UsingTiingoCryptoWebSocket"); // 是否使用Tiingo的WebSocket
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_bUsingTiingoForexWebSocket = m_systemConfiguration.at("WebSocket").at("UsingTiingoForexWebSocket"); // 是否使用Tiingo的WebSocket
		}
		catch (json::out_of_range&) { m_fUpdate = true; }

		// Financial Data Update Rate
		try { m_iStockBasicFinancialUpdateRate = m_systemConfiguration.at("FinancialDataUpdateRate").at("StockBasicFinancial"); }
		catch (json::out_of_range&) { m_fUpdate = true; }
		try { m_iStockProfileUpdateRate = m_systemConfiguration.at("FinancialDataUpdateRate").at("StockProfile"); }
		catch (json::out_of_range&) { m_fUpdate = true; }
		try { m_iInsideTransactionUpdateRate = m_systemConfiguration.at("FinancialDataUpdateRate").at("InsideTransaction"); }
		catch (json::out_of_range&) { m_fUpdate = true; }
		try { m_iInsideSentimentUpdateRate = m_systemConfiguration.at("FinancialDataUpdateRate").at("InsideSentiment"); }
		catch (json::out_of_range&) { m_fUpdate = true; }
		try { m_iStockPeerUpdateRate = m_systemConfiguration.at("FinancialDataUpdateRate").at("StockPeer"); }
		catch (json::out_of_range&) { m_fUpdate = true; }

		// 测试系统选项
		try {
			sTemp = m_systemConfiguration.at("TestConfiguration").at("BenchmarkTestFileDirectory");
			m_strBenchmarkTestFileDirectory = sTemp.c_str();
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
	}
	catch (json::exception& e) {
		TRACE("%s\n", e.what());
		ASSERT(0);
	}
}

void CSystemConfiguration::UpdateJson(void) {
	m_systemConfiguration.clear(); // 清除之前的数据。
	// system
	m_systemConfiguration["SystemConfiguration"]["UsingFastCPU"] = m_bUsingFastCPU;
	m_systemConfiguration["SystemConfiguration"]["DebugMode"] = m_bDebugMode;
	m_systemConfiguration["SystemConfiguration"]["DatabaseAccountName"] = m_strDatabaseAccountName;
	m_systemConfiguration["SystemConfiguration"]["DatabaseAccountPassword"] = m_strDatabaseAccountPassword;
	m_systemConfiguration["SystemConfiguration"]["BackgroundThreadPermittedNumber"] = m_iBackgroundThreadPermittedNumber;
	m_systemConfiguration["SystemConfiguration"]["SavingThreadPermittedNumber"] = m_iSavingThreadPermittedNumber;

	// China market
	switch (m_iChinaMarketRealtimeServer) {
	case 0:
		m_systemConfiguration["ChinaMarket"]["RealtimeServer"] = _T("sina");
		break;
	case 1:
		m_systemConfiguration["ChinaMarket"]["RealtimeServer"] = _T("netease");
		break;
	case 2:
		m_systemConfiguration["ChinaMarket"]["RealtimeServer"] = _T("tencent");
		break;
	default:
		m_systemConfiguration["ChinaMarket"]["RealtimeServer"] = _T("sina");
		break;
	}
	m_systemConfiguration["ChinaMarket"]["RealtimeInquiryTime"] = m_iChinaMarketRTDataInquiryTime;
	m_systemConfiguration["ChinaMarket"]["SavingStockDayLineThread"] = m_iSavingChinaMarketStockDayLineThread;
	m_systemConfiguration["ChinaMarket"]["SinaRTDataInquiryPerTime"] = m_iSinaRTDataInquiryPerTime;
	m_systemConfiguration["ChinaMarket"]["NeteaseRTDataInquiryPerTime"] = m_iNeteaseRTDataInquiryPerTime;
	m_systemConfiguration["ChinaMarket"]["TengxunRTDataInquiryPerTime"] = m_iTengxunRTDataInquiryPerTime;

	// World market
	m_systemConfiguration["WorldMarket"]["FinnhubToken"] = m_strFinnhubToken;
	m_systemConfiguration["WorldMarket"]["FinnhubAccountFeePaid"] = m_bFinnhubAccountFeePaid;
	m_systemConfiguration["WorldMarket"]["TiingoToken"] = m_strTiingoToken;
	m_systemConfiguration["WorldMarket"]["TiingoAccountFeePaid"] = m_bTiingoAccountFeePaid;
	m_systemConfiguration["WorldMarket"]["QuandlToken"] = m_strQuandlToken;
	m_systemConfiguration["WorldMarket"]["QuandlAccountFeePaid"] = m_bQuandlAccountFeePaid;
	m_systemConfiguration["WorldMarket"]["FinnhubInquiryTime"] = m_iWorldMarketFinnhubInquiryTime;
	m_systemConfiguration["WorldMarket"]["TiingoInquiryTime"] = m_iWorldMarketTiingoInquiryTime;
	m_systemConfiguration["WorldMarket"]["QuandlInquiryTime"] = m_iWorldMarketQuandlInquiryTime;

	// Web socket
	m_systemConfiguration["WebSocket"]["UsingFinnhubWebSocket"] = m_bUsingFinnhubWebSocket;
	m_systemConfiguration["WebSocket"]["UsingTiingoIEXWebSocket"] = m_bUsingTiingoIEXWebSocket;
	m_systemConfiguration["WebSocket"]["UsingTiingoCryptoWebSocket"] = m_bUsingTiingoCryptoWebSocket;
	m_systemConfiguration["WebSocket"]["UsingTiingoForexWebSocket"] = m_bUsingTiingoForexWebSocket;

	m_systemConfiguration["FinancialDataUpdateRate"]["StockProfile"] = m_iStockProfileUpdateRate;
	m_systemConfiguration["FinancialDataUpdateRate"]["StockBasicFinancial"] = m_iStockBasicFinancialUpdateRate;
	m_systemConfiguration["FinancialDataUpdateRate"]["InsideTransaction"] = m_iInsideTransactionUpdateRate;
	m_systemConfiguration["FinancialDataUpdateRate"]["InsideSentiment"] = m_iInsideSentimentUpdateRate;
	m_systemConfiguration["FinancialDataUpdateRate"]["StockPeer"] = m_iStockPeerUpdateRate;

	// 测试系统选项
	m_systemConfiguration["TestConfiguration"]["BenchmarkTestFileDirectory"] = m_strBenchmarkTestFileDirectory;
}

void CSystemConfiguration::UpdateSystem(void) {
	if (m_iBackgroundThreadPermittedNumber > 8) {
		for (int i = 8; i < m_iSavingThreadPermittedNumber; i++) {
			gl_BackGroundTaskThread.release();
		}
	}
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

bool CSystemConfiguration::LoadDB() {
	fstream f(GetDefaultFileDirectoryAndName(), ios::in);
	if (f.is_open()) {
		f >> m_systemConfiguration;
		//m_systemConfiguration = json::parse(f); // 这种方式等价于 f >> m_systemConfiguration;
		f.close();
		return true;
	}
	return false;
}

bool CSystemConfiguration::SaveDB() {
	fstream f(GetDefaultFileDirectoryAndName(), ios::out);
	f << m_systemConfiguration;
	f.close();

	return true;
}
