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
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // 密钥放在最后
// 下面的是第二个,用于dell240工作机。
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // 密钥放在最后
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"globedef.h"

#include"ChinaMarket.h"

#include"SystemOption.h"

#include "nlohmann/json.hpp"
using json = nlohmann::ordered_json;

using namespace std;
#include<iostream>
#include<fstream>

/// <summary>
/// 系统配置文件，采用nlohmann json库格式。
///
/// </summary>
string gl_sSystemOption = R"(
{
"ChinaMarketRealtimeServer" : 0,
"ChinaMarketRealtimeInquiryTime" : 200,

"FinnhubToken" : "&token=bv985d748v6ujthqfke0",
"TiingoToken" : "&token=c897a00b7cfc2adffc630d23befd5316a4683156",
"QuandlToken" : "&api_key=zBMXMyoTyiy_N3pMb3ex",
"WorldMarketFinnhubInquiryTime" : 1200,
"WorldMarketTiingoInquiryTime" : 9000,
"WorldMarketQuandlInquiryTime" : 36000,
"WebSocket" : {
	"UsingFinnhubWebSocket" : true,
	"UsingTiingoIEXWebSocket" : true,
	"UsingTiingoCryptoWebSocket" : true,
	"UsingTiingoForexWebSocket" : true
},
"ChinaMarket" : {
	"FastInquiringRealtimeData" : false
}
})";

string gl_sSystemConfiguration = R"(
{
"SinaRealtimeDataInquiryNumberPerTime" : 850,
"NeteaseRealtimeDataInquiryNumberPerTime" : 900,
"TengxunRealtimeDataInquiryNumberPerTime" : 900,
"FinnhubRealtimeDataInquiryNumberPerTime" : 1,
"TiingoRealtimeDataInquiryNumberPerTime" : 1,
"QuandlRealtimeDataInquiryNumberPerTime" : 1
}
)";

CSystemOption::CSystemOption() {
	static int siInstance = 0;

	if (++siInstance > 1) {
		TRACE(_T("GlobeOption全局变量只允许存在一个实例\n"));
#ifdef _DEBUG
		ASSERT(FALSE);
#endif // _DEBUG
	}

	m_fUpdate = false; // update flag
	m_fInitialized = true;
	m_strFileName = "C:\\StockAnalysis\\SystemOption.ini"; // json file name

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

	LoadDB();
	Update();
}

CSystemOption::~CSystemOption() {
	CString strOld = m_strFileName.Left(m_strFileName.GetLength() - 3) + _T("ini ");
	CString strNew = m_strFileName.Left(m_strFileName.GetLength() - 3) + _T("bak");

	if (m_fUpdate) {
		rename(strOld, strNew); // 保存备份
	}
	UpdateJson();
	SaveDB();
}

void CSystemOption::Update() {
	string sTemp;

	try {
		// ChinaMarket
		try {
			m_iChinaMarketRealtimeServer = m_systemOption.at("ChinaMarketRealtimeServer"); // 实时数据服务器选择.0:新浪实时数据；1：网易实时数据；2：腾讯实时数据（目前不使用）。
		}
		catch (json::out_of_range&) {}

		try {
			m_iChinaMarketRealtimeInquiryTime = m_systemOption.at("ChinaMarketRealtimeInquiryTime"); // 实时数据查询时间间隔（单位：毫秒）
		}
		catch (json::out_of_range&) {}

		// WorldMarket
		try {
			sTemp = m_systemOption.at("FinnhubToken"); // Finnhub token
			m_strFinnhubToken = sTemp.c_str();
		}
		catch (json::out_of_range&) {}

		try {
			sTemp = m_systemOption.at("TiingoToken"); // Tiingo token
			m_strTiingoToken = sTemp.c_str();
		}
		catch (json::out_of_range&) {}

		try {
			sTemp = m_systemOption.at("QuandlToken"); // Quandl token
			m_strQuandlToken = sTemp.c_str();
		}
		catch (json::out_of_range&) {}

		try {
			m_iWorldMarketFinnhubInquiryTime = m_systemOption.at("WorldMarketFinnhubInquiryTime"); // 默认每小时最多查询3000次
		}
		catch (json::out_of_range&) {}

		try {
			m_iWorldMarketTiingoInquiryTime = m_systemOption.at("WorldMarketTiingoInquiryTime"); // 默认每小时最多查询400次
		}
		catch (json::out_of_range&) {}

		try {
			m_iWorldMarketQuandlInquiryTime = m_systemOption.at("WorldMarketQuandlInquiryTime"); // 默认每小时最多查询100次
		}
		catch (json::out_of_range&) {}

		// WebSocket
		try {
			//m_bUsingFinnhubWebSocket = m_systemOption.at(json::json_pointer("/WebSocket/UsingFinnhubWebSocket")); // 是否使用Finnhub的WebSocket
			m_bUsingFinnhubWebSocket = m_systemOption.at("WebSocket").at("UsingFinnhubWebSocket"); // 是否使用Finnhub的WebSocket
		}
		catch (json::out_of_range&) {}

		try {
			//m_bUsingTiingoIEXWebSocket = m_systemOption.at(json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")); // 是否使用Tiingo的WebSocket
			m_bUsingTiingoIEXWebSocket = m_systemOption.at("WebSocket").at("UsingTiingoIEXWebSocket"); // 是否使用Tiingo的WebSocket
		}
		catch (json::out_of_range&) {}

		try {
			//m_bUsingTiingoCryptoWebSocket = m_systemOption.at(json::json_pointer("/WebSocket/UsingTiingoCryptoWebSocket")); // 是否使用Tiingo的WebSocket
			m_bUsingTiingoCryptoWebSocket = m_systemOption.at("WebSocket").at("UsingTiingoCryptoWebSocket"); // 是否使用Tiingo的WebSocket
		}
		catch (json::out_of_range&) {}

		try {
			//m_bUsingTiingoForexWebSocket = m_systemOption.at(json::json_pointer("/WebSocket/UsingTiingoForexWebSocket")); // 是否使用Tiingo的WebSocket
			m_bUsingTiingoForexWebSocket = m_systemOption.at("WebSocket").at("UsingTiingoForexWebSocket"); // 是否使用Tiingo的WebSocket
		}
		catch (json::out_of_range&) {}
	}
	catch (json::type_error&) {
		ASSERT(0);
	}
}

void CSystemOption::UpdateJson(void) {
	m_systemOption["ChinaMarketRealtimeServer"] = m_iChinaMarketRealtimeServer;
	m_systemOption["ChinaMarketRealtimeInquiryTime"] = m_iChinaMarketRealtimeInquiryTime;

	m_systemOption["FinnhubToken"] = m_strFinnhubToken;
	m_systemOption["TiingoToken"] = m_strTiingoToken;
	m_systemOption["QuandlToken"] = m_strQuandlToken;
	m_systemOption["WorldMarketFinnhubInquiryTime"] = m_iWorldMarketFinnhubInquiryTime;
	m_systemOption["WorldMarketTiingoInquiryTime"] = m_iWorldMarketTiingoInquiryTime;
	m_systemOption["WorldMarketQuandlInquiryTime"] = m_iWorldMarketQuandlInquiryTime;

	m_systemOption[json::json_pointer("/WebSocket/UsingFinnhubWebSocket")] = m_bUsingFinnhubWebSocket;
	m_systemOption[json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")] = m_bUsingTiingoIEXWebSocket;
	m_systemOption[json::json_pointer("/WebSocket/UsingTiingoCryptoWebSocket")] = m_bUsingTiingoCryptoWebSocket;
	m_systemOption[json::json_pointer("/WebSocket/UsingTiingoForexWebSocket")] = m_bUsingTiingoForexWebSocket;
}

bool CSystemOption::LoadDB() {
	fstream f(m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_systemOption;
	}

	return true;
}

bool CSystemOption::SaveDB() {
	fstream f(m_strFileName, ios::out);
	f << m_systemOption;
	f.close();

	return true;
}