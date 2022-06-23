///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Finnhub密钥：
// 1："&token=bv985d748v6ujthqfke0"
// 2："&token=c1i57rv48v6vit20lrc0"
//
// Tiingo密钥：
// 1："&token=c897a00b7cfc2adffc630d23befd5316a4683156"
// 2："&token=fad87279362b9e580e4fb364a263cda3c67336c8"
//
// Quandl密钥：
//  api_key=zBMXMyoTyiy_N3pMb3ex
// m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // 密钥放在最后
// 下面的是第二个,用于dell240工作机。
// m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // 密钥放在最后
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"SemaphoreDef.h"
#include"ChinaMarket.h"

#include"SystemConfigeration.h"

using namespace std;
#include<string>
#include<iostream>
#include<fstream>

/// <summary>
/// 系统配置文件，采用nlohmann json库格式。
///
/// </summary>
std::string gl_sSystemConfigeration = R"(
{
"SystemConfigeration": {
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
	"RealtimeServer" : 0,
	"RealtimeInquiryTime" : 200,
	"FastInquiringRealtimeData" : false
},

"WorldMarket" : {
	"FinnhubToken" : "&token=bv985d748v6ujthqfke0",
	"TiingoToken" : "&token=c897a00b7cfc2adffc630d23befd5316a4683156",
	"QuandlToken" : "&api_key=zBMXMyoTyiy_N3pMb3ex",
	"FinnhubInquiryTime" : 1200,
	"TiingoInquiryTime" : 9000,
	"QuandlInquiryTime" : 36000
}
})";

CSystemConfigeration::CSystemConfigeration() {
	static int siInstance = 0;

	if (++siInstance > 1) {
		TRACE(_T("GlobeOption全局变量只允许存在一个实例\n"));
#ifdef _DEBUG
		ASSERT(FALSE);
#endif // _DEBUG
	}

	m_fUpdate = false; // update flag
	m_fInitialized = true;
	m_strFileName = _T("C:\\StockAnalysis\\SystemConfigeration.json"); // json file name

	// 系统配置
	m_iBackgroundThreadPermittedNumber = 8; // 后台线程最多8个
	m_iSavingThreadPermittedNumber = 4; // 默认存储线程数为4

	// China Market
	m_iChinaMarketRealtimeServer = 0; // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据（目前不使用）。
	m_iChinaMarketRealtimeInquiryTime = 200; // 默认实时数据查询时间间隔为200毫秒

	// World Market
	m_strFinnhubToken = "&token=bv985d748v6ujthqfke0"; // Finnhub token
	m_strTiingoToken = "&token=c897a00b7cfc2adffc630d23befd5316a4683156"; // Tiingo token
	m_strQuandlToken = "&api_key=zBMXMyoTyiy_N3pMb3ex"; // Quandl token
	m_iWorldMarketFinnhubInquiryTime = 3600000 / 3000; // 默认每小时最多查询3000次
	m_iWorldMarketTiingoInquiryTime = 3600000 / 400; // 默认每小时最多查询400次。 默认免费账户的查询频率为每小时500次(每次7200毫秒）；付费账户为每小时20000次（每次180毫秒）
	m_iWorldMarketQuandlInquiryTime = 3600000 / 100; // 默认每小时最多查询100次

	// WebSocket
	m_bUsingFinnhubWebSocket = true; // 是否使用Finnhub的WebSocket
	m_bUsingTiingoIEXWebSocket = true; // 是否使用Tiingo的WebSocket
	m_bUsingTiingoCryptoWebSocket = true; // 是否使用Tiingo的WebSocket
	m_bUsingTiingoForexWebSocket = true; // 是否使用Tiingo的WebSocket

	// ChinaMarket
#ifdef DEBUG
	m_bFastInquiringRTData = true; // 主要用于测试。当需要测试系统实时数据接收负载时，DEBUG状态时设置为真。
#else
	m_bFastInquiringRTData = false;
#endif

	ASSERT(m_strFileName.Compare(_T("C:\\StockAnalysis\\SystemConfigeration.json")) == 0);
	if (LoadDB()) {
		Update();
	}
	else {
		m_fUpdate = true;
	}
}

CSystemConfigeration::~CSystemConfigeration() {
	CString strOld = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("json");
	CString strNew = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("bak");

	if (m_fUpdate) {
		DeleteFile(strNew);
		rename(strOld, strNew); // 保存备份
	}
	UpdateJson();
	SaveDB();
}

void CSystemConfigeration::Update() {
	string sTemp;

	try {
		// 系统配置
		try {
			m_iBackgroundThreadPermittedNumber = m_systemConfigeration.at("SystemConfigeration").at("BackgroundThreadPermittedNumber");
		}
		catch (json::out_of_range&) {}
		try {
			m_iSavingThreadPermittedNumber = m_systemConfigeration.at("SystemConfigeration").at("SavingThreadPermittedNumber");
		}
		catch (json::out_of_range&) {}

		// ChinaMarket
		try {
			m_iChinaMarketRealtimeServer = m_systemConfigeration.at("ChinaMarket").at("RealtimeServer"); // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据（目前不使用）。
		}
		catch (json::out_of_range&) {}

		try {
			m_iChinaMarketRealtimeInquiryTime = m_systemConfigeration.at("ChinaMarket").at("RealtimeInquiryTime"); // 实时数据查询时间间隔（单位：毫秒）
		}
		catch (json::out_of_range&) {}

		// WorldMarket
		try {
			sTemp = m_systemConfigeration.at("WorldMarket").at("FinnhubToken"); // Finnhub token
			m_strFinnhubToken = sTemp.c_str();
		}
		catch (json::out_of_range&) {}

		try {
			sTemp = m_systemConfigeration.at("WorldMarket").at("TiingoToken"); // Tiingo token
			m_strTiingoToken = sTemp.c_str();
		}
		catch (json::out_of_range&) {}

		try {
			sTemp = m_systemConfigeration.at("WorldMarket").at("QuandlToken"); // Quandl token
			m_strQuandlToken = sTemp.c_str();
		}
		catch (json::out_of_range&) {}

		try {
			m_iWorldMarketFinnhubInquiryTime = m_systemConfigeration.at("WorldMarket").at("FinnhubInquiryTime"); // 默认每小时最多查询3000次
		}
		catch (json::out_of_range&) {}

		try {
			m_iWorldMarketTiingoInquiryTime = m_systemConfigeration.at("WorldMarket").at("TiingoInquiryTime"); // 默认每小时最多查询400次
		}
		catch (json::out_of_range&) {}

		try {
			m_iWorldMarketQuandlInquiryTime = m_systemConfigeration.at("WorldMarket").at("QuandlInquiryTime"); // 默认每小时最多查询100次
		}
		catch (json::out_of_range&) {}

		// WebSocket
		try {
			//m_bUsingFinnhubWebSocket = m_systemConfigeration.at(json::json_pointer("/WebSocket/UsingFinnhubWebSocket")); // 是否使用Finnhub的WebSocket
			m_bUsingFinnhubWebSocket = m_systemConfigeration.at("WebSocket").at("UsingFinnhubWebSocket"); // 是否使用Finnhub的WebSocket
		}
		catch (json::out_of_range&) {}

		try {
			//m_bUsingTiingoIEXWebSocket = m_systemConfigeration.at(json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")); // 是否使用Tiingo的WebSocket
			m_bUsingTiingoIEXWebSocket = m_systemConfigeration.at("WebSocket").at("UsingTiingoIEXWebSocket"); // 是否使用Tiingo的WebSocket
		}
		catch (json::out_of_range&) {}

		try {
			//m_bUsingTiingoCryptoWebSocket = m_systemConfigeration.at(json::json_pointer("/WebSocket/UsingTiingoCryptoWebSocket")); // 是否使用Tiingo的WebSocket
			m_bUsingTiingoCryptoWebSocket = m_systemConfigeration.at("WebSocket").at("UsingTiingoCryptoWebSocket"); // 是否使用Tiingo的WebSocket
		}
		catch (json::out_of_range&) {}

		try {
			//m_bUsingTiingoForexWebSocket = m_systemConfigeration.at(json::json_pointer("/WebSocket/UsingTiingoForexWebSocket")); // 是否使用Tiingo的WebSocket
			m_bUsingTiingoForexWebSocket = m_systemConfigeration.at("WebSocket").at("UsingTiingoForexWebSocket"); // 是否使用Tiingo的WebSocket
		}
		catch (json::out_of_range&) {}
	}
	catch (json::type_error& e) {
		ASSERT(0);
	}
}

void CSystemConfigeration::UpdateJson(void) {
	m_systemConfigeration["SystemConfigeration"]["BackgroundThreadPermittedNumber"] = m_iBackgroundThreadPermittedNumber;
	m_systemConfigeration["SystemConfigeration"]["SavingThreadPermittedNumber"] = m_iSavingThreadPermittedNumber;

	m_systemConfigeration["ChinaMarket"]["RealtimeServer"] = m_iChinaMarketRealtimeServer;
	m_systemConfigeration["ChinaMarket"]["RealtimeInquiryTime"] = m_iChinaMarketRealtimeInquiryTime;

	m_systemConfigeration["WorldMarket"]["FinnhubToken"] = m_strFinnhubToken;
	m_systemConfigeration["WorldMarket"]["TiingoToken"] = m_strTiingoToken;
	m_systemConfigeration["WorldMarket"]["QuandlToken"] = m_strQuandlToken;
	m_systemConfigeration["WorldMarket"]["FinnhubInquiryTime"] = m_iWorldMarketFinnhubInquiryTime;
	m_systemConfigeration["WorldMarket"]["TiingoInquiryTime"] = m_iWorldMarketTiingoInquiryTime;
	m_systemConfigeration["WorldMarket"]["QuandlInquiryTime"] = m_iWorldMarketQuandlInquiryTime;

	m_systemConfigeration["WebSocket"]["UsingFinnhubWebSocket"] = m_bUsingFinnhubWebSocket;
	m_systemConfigeration["WebSocket"]["UsingTiingoIEXWebSocket"] = m_bUsingTiingoIEXWebSocket;
	m_systemConfigeration["WebSocket"]["UsingTiingoCryptoWebSocket"] = m_bUsingTiingoCryptoWebSocket;
	m_systemConfigeration["WebSocket"]["UsingTiingoForexWebSocket"] = m_bUsingTiingoForexWebSocket;
}

void CSystemConfigeration::UpdateSystem(void) {
	gl_SemaphoreBackGroundTaskThreads.SetMaxCount(m_iBackgroundThreadPermittedNumber);
	gl_SaveThreadPermitted.SetMaxCount(m_iSavingThreadPermittedNumber);
}

bool CSystemConfigeration::LoadDB() {
	fstream f(m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_systemConfigeration;
		return true;
	}
	return false;
}

bool CSystemConfigeration::SaveDB() {
	fstream f(m_strFileName, ios::out);
	f << m_systemConfigeration;
	f.close();

	return true;
}