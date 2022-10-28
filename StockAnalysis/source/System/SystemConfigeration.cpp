///////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������Կ���������Եģ��û���Ҫ�Լ�������Ե���Կ��
//
// Finnhub��Կ��
// 1��"&token=bv985d748v6ujthqfke0"
// 2��"&token=c1i57rv48v6vit20lrc0"
// 3: "&token=bv4ac1n48v6tcp17l5cg"
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

#include"ThreadStatus.h"
#include"ChinaMarket.h"
#include"WorldMarket.h"

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
	"UsingFastCPU" : true,
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
	"RealtimeInquiryTime" : 200,
	"SavingStockDayLineThread" : 4,
	"FastInquiringRealtimeData" : false
},

"WorldMarket" : {
	"FinnhubToken" : "&token=bv985d748v6ujthqfke0",
	"TiingoToken" : "&token=c897a00b7cfc2adffc630d23befd5316a4683156",
	"QuandlToken" : "&api_key=zBMXMyoTyiy_N3pMb3ex",
	"FinnhubInquiryTime" : 1200,
	"TiingoInquiryTime" : 9000,
	"QuandlInquiryTime" : 36000
},

"FinancialDataUpdateRate" : {
	"StockProfile" : 365,
	"BasicFinancial" : 45,
	"InsideTransaction" : 30,
	"InsideSentiment" : 30,
	"StockPeer" : 90
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
	m_strDirectory = _T("C:\\StockAnalysis\\");
	m_strFileName = _T("SystemConfigeration.json"); // json file name

	// ϵͳ����
	m_bUsingFastCPU = true; // Ĭ��ʹ�ÿ���CPU
	m_strDatabaseAccountName = _T("hxguo");
	m_strDatabaseAccountPassword = _T("hxguo");
	m_iBackgroundThreadPermittedNumber = 8; // ��̨�߳����8��
	m_iSavingThreadPermittedNumber = 4; // Ĭ�ϴ洢�߳���Ϊ4

	// China Market
	m_iChinaMarketRealtimeServer = 0; // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
	m_iChinaMarketRealtimeInquiryTime = 200; // Ĭ��ʵʱ���ݲ�ѯʱ����Ϊ200����
	m_iSavingChinaMarketStockDayLineThread = 4; // Ĭ���й���Ʊ��ʷ���ݴ洢�߳���Ϊ4
#ifdef DEBUG
	m_bFastInquiringRTData = false; // ��Ҫ���ڲ��ԡ�����Ҫ����ϵͳʵʱ���ݽ��ո���ʱ��DEBUG״̬ʱ����Ϊ�档
#else
	m_bFastInquiringRTData = false;
#endif

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

	// Data Update Rate
	m_iStockProfileUpdateRate = 365; // ��Ʊ�ſ�����Ƶ�ʣ���λΪ�졣Ĭ��Ϊ365�졣
	m_iStockBasicFinancialUpdateRate = 45; // �����������Ƶ�ʣ���λΪ�졣Ĭ��Ϊ45�졣
	m_iInsideTransactionUpdateRate = 30; // �ڲ����׸���Ƶ�ʣ���λΪ�졣Ĭ��Ϊ30�졣
	m_iInsideSentimentUpdateRate = 30; // �ڲ�������������Ƶ�ʣ���λΪ�졣Ĭ��Ϊ30�졣
	m_iStockPeerUpdateRate = 90; // ��Ʊ���ָ���Ƶ�ʣ���λΪ�졣Ĭ��Ϊ90�졣

	ASSERT(GetDefaultFileDirectoryAndName().Compare(_T("C:\\StockAnalysis\\SystemConfigeration.json")) == 0);
	if (LoadDB()) {
		Update();
	}
	else {
		m_fUpdate = true;
	}

	// ���幤��������ĳ�ʼ����
	m_rSystemDisplay.left = 0;
	m_rSystemDisplay.right = 2600;
	m_rSystemDisplay.top = 0;
	m_rSystemDisplay.bottom = 1600;
	m_rCurrentWindow.left = 0;
	m_rCurrentWindow.right = 2600;
	m_rCurrentWindow.top = 0;
	m_rCurrentWindow.bottom = 1600;
}

CSystemConfigeration::~CSystemConfigeration() {
	CString strOld = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("json");
	CString strNew = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("bak");

	if (m_fUpdate) {
		DeleteFile(GetDefaultFileDirectory() + strNew);
		rename(GetDefaultFileDirectory() + strOld, GetDefaultFileDirectory() + strNew); // ���汸��
	}
	UpdateJson();
	SaveDB();
}

void CSystemConfigeration::Update() {
	string sTemp;

	try {
		// ϵͳ����
		try {
			m_bUsingFastCPU = m_systemConfigeration.at("SystemConfigeration").at("UsingFastCPU");
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			sTemp = m_systemConfigeration.at("SystemConfigeration").at("DatabaseAccountName");
			m_strDatabaseAccountName = sTemp.c_str();
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			sTemp = m_systemConfigeration.at("SystemConfigeration").at("DatabaseAccountPassword");
			m_strDatabaseAccountPassword = sTemp.c_str();
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iBackgroundThreadPermittedNumber = m_systemConfigeration.at("SystemConfigeration").at("BackgroundThreadPermittedNumber");
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iSavingThreadPermittedNumber = m_systemConfigeration.at("SystemConfigeration").at("SavingThreadPermittedNumber");
		}
		catch (json::out_of_range&) { m_fUpdate = true; }

		// ChinaMarket
		try {
			sTemp = m_systemConfigeration.at("ChinaMarket").at("RealtimeServer");// ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���

			if (sTemp.compare(_T("sina")) == 0) {
				m_iChinaMarketRealtimeServer = 0;
			}
			else if (sTemp.compare(_T("netease")) == 0) {
				m_iChinaMarketRealtimeServer = 1;
			}
			else { // �Ƿ����������ƣ�ʹ��Ĭ��sina������
				m_iChinaMarketRealtimeServer = 0;
				m_fUpdate = true;
			}
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iChinaMarketRealtimeInquiryTime = m_systemConfigeration.at("ChinaMarket").at("RealtimeInquiryTime"); // ʵʱ���ݲ�ѯʱ��������λ�����룩
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iSavingChinaMarketStockDayLineThread = m_systemConfigeration.at("ChinaMarket").at("SavingStockDayLineThread"); // �����Ʊ���������߳�����
		}
		catch (json::out_of_range&) { m_fUpdate = true; }

		// WorldMarket
		try {
			sTemp = m_systemConfigeration.at("WorldMarket").at("FinnhubToken"); // Finnhub token
			m_strFinnhubToken = sTemp.c_str();
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			sTemp = m_systemConfigeration.at("WorldMarket").at("TiingoToken"); // Tiingo token
			m_strTiingoToken = sTemp.c_str();
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			sTemp = m_systemConfigeration.at("WorldMarket").at("QuandlToken"); // Quandl token
			m_strQuandlToken = sTemp.c_str();
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iWorldMarketFinnhubInquiryTime = m_systemConfigeration.at("WorldMarket").at("FinnhubInquiryTime"); // Ĭ��ÿСʱ����ѯ3000��
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iWorldMarketTiingoInquiryTime = m_systemConfigeration.at("WorldMarket").at("TiingoInquiryTime"); // Ĭ��ÿСʱ����ѯ400��
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iWorldMarketQuandlInquiryTime = m_systemConfigeration.at("WorldMarket").at("QuandlInquiryTime"); // Ĭ��ÿСʱ����ѯ100��
		}
		catch (json::out_of_range&) { m_fUpdate = true; }

		// WebSocket
		try {
			m_bUsingFinnhubWebSocket = m_systemConfigeration.at("WebSocket").at("UsingFinnhubWebSocket"); // �Ƿ�ʹ��Finnhub��WebSocket
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_bUsingTiingoIEXWebSocket = m_systemConfigeration.at("WebSocket").at("UsingTiingoIEXWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_bUsingTiingoCryptoWebSocket = m_systemConfigeration.at("WebSocket").at("UsingTiingoCryptoWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_bUsingTiingoForexWebSocket = m_systemConfigeration.at("WebSocket").at("UsingTiingoForexWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
		}
		catch (json::out_of_range&) { m_fUpdate = true; }

		// Financial Data Update Rate
		try {
			m_iStockBasicFinancialUpdateRate = m_systemConfigeration.at("FinancialDataUpdateRate").at("StockBasicFinancial");
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iStockProfileUpdateRate = m_systemConfigeration.at("FinancialDataUpdateRate").at("StockProfile");
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iInsideTransactionUpdateRate = m_systemConfigeration.at("FinancialDataUpdateRate").at("InsideTransaction");
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iInsideSentimentUpdateRate = m_systemConfigeration.at("FinancialDataUpdateRate").at("InsideSentiment");
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
		try {
			m_iStockPeerUpdateRate = m_systemConfigeration.at("FinancialDataUpdateRate").at("StockPeer");
		}
		catch (json::out_of_range&) { m_fUpdate = true; }
	}
	catch (json::type_error& e) {
		TRACE("%s\n", e.what());
		ASSERT(0);
	}
}

void CSystemConfigeration::UpdateJson(void) {
	m_systemConfigeration["SystemConfigeration"]["UsingFastCPU"] = m_bUsingFastCPU;
	m_systemConfigeration["SystemConfigeration"]["DatabaseAccountName"] = m_strDatabaseAccountName;
	m_systemConfigeration["SystemConfigeration"]["DatabaseAccountPassword"] = m_strDatabaseAccountPassword;
	m_systemConfigeration["SystemConfigeration"]["BackgroundThreadPermittedNumber"] = m_iBackgroundThreadPermittedNumber;
	m_systemConfigeration["SystemConfigeration"]["SavingThreadPermittedNumber"] = m_iSavingThreadPermittedNumber;

	switch (m_iChinaMarketRealtimeServer) {
	case 0:
		m_systemConfigeration["ChinaMarket"]["RealtimeServer"] = _T("sina");
		break;
	case 1:
		m_systemConfigeration["ChinaMarket"]["RealtimeServer"] = _T("netease");
		break;
	case 2:
		m_systemConfigeration["ChinaMarket"]["RealtimeServer"] = _T("tencent");
		break;
	default:
		m_systemConfigeration["ChinaMarket"]["RealtimeServer"] = _T("sina");
		break;
	}
	m_systemConfigeration["ChinaMarket"]["RealtimeInquiryTime"] = m_iChinaMarketRealtimeInquiryTime;
	m_systemConfigeration["ChinaMarket"]["SavingStockDayLineThread"] = m_iSavingChinaMarketStockDayLineThread;

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

	m_systemConfigeration["FinancialDataUpdateRate"]["StockProfile"] = m_iStockProfileUpdateRate;
	m_systemConfigeration["FinancialDataUpdateRate"]["StockBasicFinancial"] = m_iStockBasicFinancialUpdateRate;
	m_systemConfigeration["FinancialDataUpdateRate"]["InsideTransaction"] = m_iInsideTransactionUpdateRate;
	m_systemConfigeration["FinancialDataUpdateRate"]["InsideSentiment"] = m_iInsideSentimentUpdateRate;
	m_systemConfigeration["FinancialDataUpdateRate"]["StockPeer"] = m_iStockPeerUpdateRate;
}

void CSystemConfigeration::UpdateSystem(void) {
	if (m_iBackgroundThreadPermittedNumber > 8) {
		for (int i = 8; i < m_iSavingThreadPermittedNumber; i++) {
			gl_BackGroundTaskThread.release();
		}
	}
}

bool CSystemConfigeration::LoadDB() {
	fstream f(GetDefaultFileDirectoryAndName(), ios::in);
	if (f.is_open()) {
		f >> m_systemConfigeration;
		//m_systemConfigeration = json::parse(f); // ���ַ�ʽ�ȼ��� f >> m_systemConfigeration;
		return true;
	}
	return false;
}

bool CSystemConfigeration::SaveDB() {
	fstream f(GetDefaultFileDirectoryAndName(), ios::out);
	f << m_systemConfigeration;
	f.close();

	return true;
}