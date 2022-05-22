///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Finnhub��Կ��
// 1��"&token=bv985d748v6ujthqfke0"
// 2��"&token=c1i57rv48v6vit20lrc0"
//
// Tiingo��Կ��
// 1��"&token=c897a00b7cfc2adffc630d23befd5316a4683156"
// 2��"&token=fad87279362b9e580e4fb364a263cda3c67336c8"
//
// Quandl��Կ��
//  api_key=zBMXMyoTyiy_N3pMb3ex
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ��Կ�������
// ������ǵڶ���,����dell240��������
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ��Կ�������
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
/// ϵͳ�����ļ�������nlohmann json���ʽ��
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
		TRACE(_T("GlobeOptionȫ�ֱ���ֻ�������һ��ʵ��\n"));
#ifdef _DEBUG
		ASSERT(FALSE);
#endif // _DEBUG
	}

	m_fUpdate = false; // update flag
	m_fInitialized = true;
	m_strFileName = "C:\\StockAnalysis\\SystemOption.ini"; // json file name

	// China Market
	m_iChinaMarketRealtimeServer = 0; // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
	m_iChinaMarketRealtimeInquiryTime = 200; // Ĭ��ʵʱ���ݲ�ѯʱ����Ϊ200����

	// World Market
	m_strFinnhubToken = "&token=bv985d748v6ujthqfke0"; // Finnhub token
	m_strTiingoToken = "&token=c897a00b7cfc2adffc630d23befd5316a4683156"; // Tiingo token
	m_strQuandlToken = "&api_key=zBMXMyoTyiy_N3pMb3ex"; // Quandl token
	m_iWorldMarketFinnhubInquiryTime = 3600000 / 3000; // Ĭ��ÿСʱ����ѯ3000��
	m_iWorldMarketTiingoInquiryTime = 3600000 / 400; // Ĭ��ÿСʱ����ѯ400�Ρ� Ĭ������˻��Ĳ�ѯƵ��ΪÿСʱ500��(ÿ��7200���룩�������˻�ΪÿСʱ20000�Σ�ÿ��180���룩
	m_iWorldMarketQuandlInquiryTime = 3600000 / 100; // Ĭ��ÿСʱ����ѯ100��

	// WebSocket
	m_bUsingFinnhubWebSocket = true; // �Ƿ�ʹ��Finnhub��WebSocket
	m_bUsingTiingoIEXWebSocket = true; // �Ƿ�ʹ��Tiingo��WebSocket
	m_bUsingTiingoCryptoWebSocket = true; // �Ƿ�ʹ��Tiingo��WebSocket
	m_bUsingTiingoForexWebSocket = true; // �Ƿ�ʹ��Tiingo��WebSocket

	// ChinaMarket
#ifdef DEBUG
	m_bFastInquiringRTData = true; // ��Ҫ���ڲ��ԡ�����Ҫ����ϵͳʵʱ���ݽ��ո���ʱ��DEBUG״̬ʱ����Ϊ�档
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
		rename(strOld, strNew); // ���汸��
	}
	UpdateJson();
	SaveDB();
}

void CSystemOption::Update() {
	string sTemp;

	try {
		// ChinaMarket
		try {
			m_iChinaMarketRealtimeServer = m_systemOption.at("ChinaMarketRealtimeServer"); // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
		}
		catch (json::out_of_range&) {}

		try {
			m_iChinaMarketRealtimeInquiryTime = m_systemOption.at("ChinaMarketRealtimeInquiryTime"); // ʵʱ���ݲ�ѯʱ��������λ�����룩
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
			m_iWorldMarketFinnhubInquiryTime = m_systemOption.at("WorldMarketFinnhubInquiryTime"); // Ĭ��ÿСʱ����ѯ3000��
		}
		catch (json::out_of_range&) {}

		try {
			m_iWorldMarketTiingoInquiryTime = m_systemOption.at("WorldMarketTiingoInquiryTime"); // Ĭ��ÿСʱ����ѯ400��
		}
		catch (json::out_of_range&) {}

		try {
			m_iWorldMarketQuandlInquiryTime = m_systemOption.at("WorldMarketQuandlInquiryTime"); // Ĭ��ÿСʱ����ѯ100��
		}
		catch (json::out_of_range&) {}

		// WebSocket
		try {
			//m_bUsingFinnhubWebSocket = m_systemOption.at(json::json_pointer("/WebSocket/UsingFinnhubWebSocket")); // �Ƿ�ʹ��Finnhub��WebSocket
			m_bUsingFinnhubWebSocket = m_systemOption.at("WebSocket").at("UsingFinnhubWebSocket"); // �Ƿ�ʹ��Finnhub��WebSocket
		}
		catch (json::out_of_range&) {}

		try {
			//m_bUsingTiingoIEXWebSocket = m_systemOption.at(json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")); // �Ƿ�ʹ��Tiingo��WebSocket
			m_bUsingTiingoIEXWebSocket = m_systemOption.at("WebSocket").at("UsingTiingoIEXWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
		}
		catch (json::out_of_range&) {}

		try {
			//m_bUsingTiingoCryptoWebSocket = m_systemOption.at(json::json_pointer("/WebSocket/UsingTiingoCryptoWebSocket")); // �Ƿ�ʹ��Tiingo��WebSocket
			m_bUsingTiingoCryptoWebSocket = m_systemOption.at("WebSocket").at("UsingTiingoCryptoWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
		}
		catch (json::out_of_range&) {}

		try {
			//m_bUsingTiingoForexWebSocket = m_systemOption.at(json::json_pointer("/WebSocket/UsingTiingoForexWebSocket")); // �Ƿ�ʹ��Tiingo��WebSocket
			m_bUsingTiingoForexWebSocket = m_systemOption.at("WebSocket").at("UsingTiingoForexWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
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