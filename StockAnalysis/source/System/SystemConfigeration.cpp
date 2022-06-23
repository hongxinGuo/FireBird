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
// m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ��Կ�������
// ������ǵڶ���,����dell240��������
// m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ��Կ�������
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
/// ϵͳ�����ļ�������nlohmann json���ʽ��
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
		TRACE(_T("GlobeOptionȫ�ֱ���ֻ�������һ��ʵ��\n"));
#ifdef _DEBUG
		ASSERT(FALSE);
#endif // _DEBUG
	}

	m_fUpdate = false; // update flag
	m_fInitialized = true;
	m_strFileName = _T("C:\\StockAnalysis\\SystemConfigeration.json"); // json file name

	// ϵͳ����
	m_iBackgroundThreadPermittedNumber = 8; // ��̨�߳����8��
	m_iSavingThreadPermittedNumber = 4; // Ĭ�ϴ洢�߳���Ϊ4

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
		rename(strOld, strNew); // ���汸��
	}
	UpdateJson();
	SaveDB();
}

void CSystemConfigeration::Update() {
	string sTemp;

	try {
		// ϵͳ����
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
			m_iChinaMarketRealtimeServer = m_systemConfigeration.at("ChinaMarket").at("RealtimeServer"); // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
		}
		catch (json::out_of_range&) {}

		try {
			m_iChinaMarketRealtimeInquiryTime = m_systemConfigeration.at("ChinaMarket").at("RealtimeInquiryTime"); // ʵʱ���ݲ�ѯʱ��������λ�����룩
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
			m_iWorldMarketFinnhubInquiryTime = m_systemConfigeration.at("WorldMarket").at("FinnhubInquiryTime"); // Ĭ��ÿСʱ����ѯ3000��
		}
		catch (json::out_of_range&) {}

		try {
			m_iWorldMarketTiingoInquiryTime = m_systemConfigeration.at("WorldMarket").at("TiingoInquiryTime"); // Ĭ��ÿСʱ����ѯ400��
		}
		catch (json::out_of_range&) {}

		try {
			m_iWorldMarketQuandlInquiryTime = m_systemConfigeration.at("WorldMarket").at("QuandlInquiryTime"); // Ĭ��ÿСʱ����ѯ100��
		}
		catch (json::out_of_range&) {}

		// WebSocket
		try {
			//m_bUsingFinnhubWebSocket = m_systemConfigeration.at(json::json_pointer("/WebSocket/UsingFinnhubWebSocket")); // �Ƿ�ʹ��Finnhub��WebSocket
			m_bUsingFinnhubWebSocket = m_systemConfigeration.at("WebSocket").at("UsingFinnhubWebSocket"); // �Ƿ�ʹ��Finnhub��WebSocket
		}
		catch (json::out_of_range&) {}

		try {
			//m_bUsingTiingoIEXWebSocket = m_systemConfigeration.at(json::json_pointer("/WebSocket/UsingTiingoIEXWebSocket")); // �Ƿ�ʹ��Tiingo��WebSocket
			m_bUsingTiingoIEXWebSocket = m_systemConfigeration.at("WebSocket").at("UsingTiingoIEXWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
		}
		catch (json::out_of_range&) {}

		try {
			//m_bUsingTiingoCryptoWebSocket = m_systemConfigeration.at(json::json_pointer("/WebSocket/UsingTiingoCryptoWebSocket")); // �Ƿ�ʹ��Tiingo��WebSocket
			m_bUsingTiingoCryptoWebSocket = m_systemConfigeration.at("WebSocket").at("UsingTiingoCryptoWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
		}
		catch (json::out_of_range&) {}

		try {
			//m_bUsingTiingoForexWebSocket = m_systemConfigeration.at(json::json_pointer("/WebSocket/UsingTiingoForexWebSocket")); // �Ƿ�ʹ��Tiingo��WebSocket
			m_bUsingTiingoForexWebSocket = m_systemConfigeration.at("WebSocket").at("UsingTiingoForexWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
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