///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ChinaMarket.h"

#include"SystemConfiguration.h"

#include<string>
#include<fstream>

#include "TengxunRTDataSource.h"
using std::fstream;
using std::ios;

bool CSystemConfiguration::sm_bInitialized = false;

/// <summary>
/// ϵͳ�����ļ�������nlohmann json���ʽ��
///
/// </summary>
std::string gl_sSystemConfiguration = R"(
{
"SystemConfiguration": {
	"UsingFastCPU" : true,
	"DebugMode" : false,
	"ReloadSystem" : false,
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

// ȷ��SystemConfiguration�ǵ�һ����ʼ����ȫ�ֱ�����������ȫ�ֱ������ܻ�ʹ�øñ��������ݡ�
CSystemConfiguration::CSystemConfiguration() {
	ASSERT(!sm_bInitialized); // ֻ����Ψһʵ��
	if (sm_bInitialized) {
		TRACE(_T("GlobeOptionȫ�ֱ���ֻ�������һ��ʵ��\n"));
#ifdef _DEBUG
		ASSERT(FALSE);
#endif // _DEBUG
	}
	sm_bInitialized = true;

	m_fUpdate = false; // update flag
	m_fInitialized = true;
	m_strDirectory = _T("C:\\FireBird\\");
	m_strFileName = _T("SystemConfiguration.json"); // json file name

	// ϵͳ����
	m_bDebugMode = false;
	m_bReloadSystem = false;
	m_strDatabaseAccountName = _T("hxguo");
	m_strDatabaseAccountPassword = _T("hxguo");
	m_iBackgroundThreadPermittedNumber = 8; // ��̨�߳����8��

	// China Market
	m_iChinaMarketRealtimeServer = 0; // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݡ�
	m_iChinaMarketDayLineServer = 0; // �������ݷ�������0:�������߷�������1����Ѷ���߷�����
	m_iChinaMarketRTDataInquiryTime = 250; // Ĭ��ʵʱ���ݲ�ѯʱ����Ϊ250����
	m_iSavingChinaMarketStockDayLineThread = 4; // Ĭ���й���Ʊ��ʷ���ݴ洢�߳���Ϊ4
#ifdef DEBUG
	m_bFastInquiringRTData = false; // ���ڲ��ԡ�����Ҫ����ϵͳʵʱ���ݽ��ո���ʱ��DEBUG״̬ʱ����Ϊ�档Ĭ��Ϊ��
#else
	m_bFastInquiringRTData = false;
#endif
	m_iNumberOfRTDataSource = 4;
	m_iSinaRTDataInquiryPerTime = 850;
	m_iNeteaseRTDataInquiryPerTime = 900;
	m_iTengxunRTDataInquiryPerTime = 900;

	// World Market
	m_strFinnhubToken = "bv985d748v6ujthqfke0"; // Finnhub token
	m_strTiingoToken = "c897a00b7cfc2adffc630d23befd5316a4683156"; // Tiingo token
	m_strQuandlToken = _T(""); // Quandl token
	m_iWorldMarketFinnhubInquiryTime = 1100; // Ĭ��ÿСʱ����ѯ3300��
	m_iWorldMarketTiingoInquiryTime = 3600000 / 500; // Ĭ��ÿСʱ����ѯ500�Ρ� Ĭ������˻��Ĳ�ѯƵ��ΪÿСʱ500��(ÿ��7200���룩�������˻�ΪÿСʱ20000�Σ�ÿ��180���룩
	m_iWorldMarketQuandlInquiryTime = 3600000 / 100; // Ĭ��ÿСʱ����ѯ100��
	m_bFinnhubAccountFeePaid = true; // Ĭ��Ϊ�����˻����ɳ�������Ƿ��Ϊ����˻����Լ���Ҳ���ԣ�
	m_bQuandlAccountFeePaid = true;// Ĭ��Ϊ�����˻����ɳ�������Ƿ��Ϊ����˻����Լ���Ҳ���ԣ�
	m_bQuandlAccountFeePaid = true;// Ĭ��Ϊ�����˻����ɳ�������Ƿ��Ϊ����˻����Լ���Ҳ���ԣ�

	// WebSocket
	m_bUsingFinnhubWebSocket = true; // �Ƿ�ʹ��Finnhub��WebSocket
	m_bUsingTiingoIEXWebSocket = true; // �Ƿ�ʹ��Tiingo��WebSocket
	m_bUsingTiingoCryptoWebSocket = true; // �Ƿ�ʹ��Tiingo��WebSocket
	m_bUsingTiingoForexWebSocket = true; // �Ƿ�ʹ��Tiingo��WebSocket
	m_bUsingQuandlWebSocket = true;

	// Data Update Rate
	m_iStockProfileUpdateRate = 365; // ��Ʊ�ſ�����Ƶ�ʣ���λΪ�졣Ĭ��Ϊ365�졣
	m_iStockBasicFinancialUpdateRate = 45; // �����������Ƶ�ʣ���λΪ�졣Ĭ��Ϊ45�졣
	m_iInsideTransactionUpdateRate = 30; // �ڲ����׸���Ƶ�ʣ���λΪ�졣Ĭ��Ϊ30�졣
	m_iInsideSentimentUpdateRate = 30; // �ڲ�������������Ƶ�ʣ���λΪ�졣Ĭ��Ϊ30�졣
	m_iStockPeerUpdateRate = 90; // ��Ʊ���ָ���Ƶ�ʣ���λΪ�졣Ĭ��Ϊ90�졣

	// ����ϵͳѡ��
	m_strBenchmarkTestFileDirectory = _T("C:\\FireBird\\Test Data\\Benchmark\\"); // BenchmarkĬ��Ŀ¼

	ASSERT(GetDefaultFileDirectoryAndName().Compare(_T("C:\\FireBird\\SystemConfiguration.json")) == 0);
	if (LoadDB()) {
		Update();
	}
	else { m_fUpdate = true; }

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

CSystemConfiguration::~CSystemConfiguration() {
	const CString strOld = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("json");
	const CString strNew = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("bak");

	if (m_fUpdate) {
		DeleteFile(GetDefaultFileDirectory() + strNew);
		rename(GetDefaultFileDirectory() + strOld, GetDefaultFileDirectory() + strNew); // ���汸��
	}
	UpdateJson();
	SaveDB();
}

void CSystemConfiguration::Update() {
	string sTemp;

	try {
		// ϵͳ����
		try {
			m_bDebugMode = m_systemConfiguration.at("SystemConfiguration").at("DebugMode");
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_bReloadSystem = m_systemConfiguration.at("SystemConfiguration").at("ReloadSystem");
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			sTemp = m_systemConfiguration.at("SystemConfiguration").at("DatabaseAccountName");
			m_strDatabaseAccountName = sTemp.c_str();
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			sTemp = m_systemConfiguration.at("SystemConfiguration").at("DatabaseAccountPassword");
			m_strDatabaseAccountPassword = sTemp.c_str();
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_iBackgroundThreadPermittedNumber = m_systemConfiguration.at("SystemConfiguration").at("BackgroundThreadPermittedNumber");
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}

		// ChinaMarket
		try {
			sTemp = m_systemConfiguration.at("ChinaMarket").at("RealtimeServer"); // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
			if (sTemp == _T("sina")) { m_iChinaMarketRealtimeServer = 0; }
			else if (sTemp == _T("netease")) { m_iChinaMarketRealtimeServer = 1; }
			else if (sTemp == _T("tengxun")) { m_iChinaMarketRealtimeServer = 2; }
			else { // �Ƿ����������ƣ�ʹ��Ĭ��sina������
				m_iChinaMarketRealtimeServer = 0;
				m_fUpdate = true;
			}
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			sTemp = m_systemConfiguration.at("ChinaMarket").at("DayLineServer"); // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
			if (sTemp == _T("netease")) { m_iChinaMarketDayLineServer = 0; }
			else if (sTemp == _T("tengxun")) { m_iChinaMarketDayLineServer = 1; }
			else {// �Ƿ����������ƣ�ʹ��Ĭ��sina������
				m_iChinaMarketDayLineServer = 0;
				m_fUpdate = true;
			}
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_iChinaMarketRTDataInquiryTime = m_systemConfiguration.at("ChinaMarket").at("RealtimeInquiryTime"); // ʵʱ���ݲ�ѯʱ��������λ�����룩
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_iSavingChinaMarketStockDayLineThread = m_systemConfiguration.at("ChinaMarket").at("SavingStockDayLineThread"); // �����Ʊ���������߳�����
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_iNumberOfRTDataSource = m_systemConfiguration.at("ChinaMarket").at("NumberOfRTDataSource"); // Sinaʵʱ��������������
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
			m_iSinaRTDataInquiryPerTime = m_systemConfiguration.at("ChinaMarket").at("SinaRTDataInquiryPerTime"); // Sinaʵʱ����ÿ�β�ѯ��Ʊ��
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_iNeteaseRTDataInquiryPerTime = m_systemConfiguration.at("ChinaMarket").at("NeteaseRTDataInquiryPerTime"); // Sinaʵʱ����ÿ�β�ѯ��Ʊ��
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_iTengxunRTDataInquiryPerTime = m_systemConfiguration.at("ChinaMarket").at("TengxunRTDataInquiryPerTime"); // Sinaʵʱ����ÿ�β�ѯ��Ʊ��
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}

		// WorldMarket
		try {
			sTemp = m_systemConfiguration.at("WorldMarket").at("FinnhubToken"); // Finnhub token
			m_strFinnhubToken = sTemp.c_str();
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_bFinnhubAccountFeePaid = m_systemConfiguration.at("WorldMarket").at("FinnhubAccountFeePaid");
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			sTemp = m_systemConfiguration.at("WorldMarket").at("TiingoToken"); // Tiingo token
			m_strTiingoToken = sTemp.c_str();
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_bTiingoAccountFeePaid = m_systemConfiguration.at("WorldMarket").at("TiingoAccountFeePaid");
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			sTemp = m_systemConfiguration.at("WorldMarket").at("QuandlToken"); // Quandl token
			m_strQuandlToken = sTemp.c_str();
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_bQuandlAccountFeePaid = m_systemConfiguration.at("WorldMarket").at("QuandlAccountFeePaid");
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_iWorldMarketFinnhubInquiryTime = m_systemConfiguration.at("WorldMarket").at("FinnhubInquiryTime"); // Ĭ��ÿСʱ����ѯ3000��
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_iWorldMarketTiingoInquiryTime = m_systemConfiguration.at("WorldMarket").at("TiingoInquiryTime"); // Ĭ��ÿСʱ����ѯ400��
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_iWorldMarketQuandlInquiryTime = m_systemConfiguration.at("WorldMarket").at("QuandlInquiryTime"); // Ĭ��ÿСʱ����ѯ100��
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}

		// WebSocket
		try {
			m_bUsingFinnhubWebSocket = m_systemConfiguration.at("WebSocket").at("UsingFinnhubWebSocket"); // �Ƿ�ʹ��Finnhub��WebSocket
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_bUsingTiingoIEXWebSocket = m_systemConfiguration.at("WebSocket").at("UsingTiingoIEXWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_bUsingTiingoCryptoWebSocket = m_systemConfiguration.at("WebSocket").at("UsingTiingoCryptoWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_bUsingTiingoForexWebSocket = m_systemConfiguration.at("WebSocket").at("UsingTiingoForexWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}

		// Financial Data Update Rate
		try {
			m_iStockBasicFinancialUpdateRate = m_systemConfiguration.at("FinancialDataUpdateRate").at("StockBasicFinancial");
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_iStockProfileUpdateRate = m_systemConfiguration.at("FinancialDataUpdateRate").at("StockProfile");
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_iInsideTransactionUpdateRate = m_systemConfiguration.at("FinancialDataUpdateRate").at("InsideTransaction");
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_iInsideSentimentUpdateRate = m_systemConfiguration.at("FinancialDataUpdateRate").at("InsideSentiment");
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}
		try {
			m_iStockPeerUpdateRate = m_systemConfiguration.at("FinancialDataUpdateRate").at("StockPeer");
		}
		catch (json::out_of_range&) {
			m_fUpdate = true;
		}

		// ����ϵͳѡ��
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

void CSystemConfiguration::UpdateJson() {
	m_systemConfiguration.clear(); // ���֮ǰ�����ݡ�
	// system
	m_systemConfiguration["SystemConfiguration"]["DebugMode"] = m_bDebugMode;
	m_systemConfiguration["SystemConfiguration"]["ReloadSystem"] = m_bReloadSystem;
	m_systemConfiguration["SystemConfiguration"]["DatabaseAccountName"] = m_strDatabaseAccountName;
	m_systemConfiguration["SystemConfiguration"]["DatabaseAccountPassword"] = m_strDatabaseAccountPassword;
	m_systemConfiguration["SystemConfiguration"]["BackgroundThreadPermittedNumber"] = m_iBackgroundThreadPermittedNumber;

	// China market
	switch (m_iChinaMarketRealtimeServer) {
	case 0:
		m_systemConfiguration["ChinaMarket"]["RealtimeServer"] = _T("sina");
		break;
	case 1:
		m_systemConfiguration["ChinaMarket"]["RealtimeServer"] = _T("netease");
		break;
	case 2:
		m_systemConfiguration["ChinaMarket"]["RealtimeServer"] = _T("tengxun");
		break;
	default:
		m_systemConfiguration["ChinaMarket"]["RealtimeServer"] = _T("sina");
		break;
	}
	switch (m_iChinaMarketDayLineServer) {
	case 0:
		m_systemConfiguration["ChinaMarket"]["DayLineServer"] = _T("netease");
		break;
	case 1:
		m_systemConfiguration["ChinaMarket"]["DayLineServer"] = _T("tengxun");
		break;
	default:
		m_systemConfiguration["ChinaMarket"]["DayLineServer"] = _T("netease");
		break;
	}
	m_systemConfiguration["ChinaMarket"]["NumberOfRTDataSource"] = m_iNumberOfRTDataSource;
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

	// ����ϵͳѡ��
	m_systemConfiguration["TestConfiguration"]["BenchmarkTestFileDirectory"] = m_strBenchmarkTestFileDirectory;
}

void CSystemConfiguration::UpdateSystem() const {}

void CSystemConfiguration::ChangeFinnhubAccountTypeToFree() {
	m_bFinnhubAccountFeePaid = false;
	m_iWorldMarketFinnhubInquiryTime = 1100; // ÿ��1100����
	m_fUpdate = true;
}

void CSystemConfiguration::ChangeFinnhubAccountTypeToPaid() {
	m_bFinnhubAccountFeePaid = true;
	m_iWorldMarketFinnhubInquiryTime = 220; // ÿ��220����
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

bool CSystemConfiguration::LoadDB() {
	fstream f(GetDefaultFileDirectoryAndName(), ios::in);
	if (f.is_open()) {
		f >> m_systemConfiguration;
		//m_systemConfiguration = json::parse(f); // ���ַ�ʽ�ȼ��� f >> m_systemConfiguration;
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
