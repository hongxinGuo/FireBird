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
/// ϵͳ�����ļ�������nlohmann json���ʽ��
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
	char buffer[200];
	_getcwd(buffer, 200);
	m_strDirectory = buffer;
	m_strDirectory = m_strDirectory + _T("\\"); //
	m_strFileName = _T("SystemConfiguration.json"); // json file name

	// ϵͳ���� 
	m_bDebugMode = false;
	m_bReloadSystem = false;
	m_strDatabaseAccountName = _T("FireBird");
	m_strDatabaseAccountPassword = _T("firebird");
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
	m_lMarketResettingTime = 170000; // Ĭ���г�����ʱ��Ϊ170000
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
	m_iEPSSurpriseUpdateRate = 90;
	m_iSECFilingsUpdateRate = 30;

	// spdlog��־�ȼ�
	m_iLogLevel = SPDLOG_LEVEL_TRACE; // Ĭ�ϼ�¼�ȼ�Ϊ���ټ���������־�Լ�¼��

	// ����ϵͳѡ��
	m_strBenchmarkTestFileDirectory = _T("C:\\FireBird\\Test Data\\Benchmark\\"); // BenchmarkĬ��Ŀ¼

	if (!LoadDB()) {
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

CSystemConfiguration::~CSystemConfiguration() {
	if (IsNeedUpdate()) {
		UpdateDB();
	}
}

void CSystemConfiguration::UpdateDB() {
	const CString strOld = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("json");
	const CString strNew = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("bak");
	DeleteFile(GetConfigurationFileDirectory() + strNew);
	rename(GetConfigurationFileDirectory() + strOld, GetConfigurationFileDirectory() + strNew); // ���汸��
	SaveDB();
	SetUpdate(false);
}

void CSystemConfiguration::Update(json& jsonData) {
	string sTemp;
	CString str1, str2, str3;
	str1.LoadString(nullptr, IDS_PROPERTYVIEW_SYSTEM_STATUS); // Note ʹ��LoadStringʱ��������ȷ����hInstanceΪnullptr�����򱨴�
	str2.LoadString(nullptr, IDS_PROPERTYVIEW_PROPERTIES_WINDOW);
	str3.LoadString(nullptr, IDS_PROPERTYVIEW_PROPERTIES2);
	// ��������
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

	// ϵͳ����
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
		sTemp = jsonData.at("ChinaMarket").at("RealtimeServer"); // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
		if (sTemp == _T("sina")) {
			m_iChinaMarketRealtimeServer = 0;
		}
		else if (sTemp == _T("netease")) {
			m_iChinaMarketRealtimeServer = 1;
		}
		else if (sTemp == _T("tengxun")) {
			m_iChinaMarketRealtimeServer = 2;
		}
		else { // �Ƿ����������ƣ�ʹ��Ĭ��sina������
			m_iChinaMarketRealtimeServer = 0;
			m_fUpdate = true;
		}
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		sTemp = jsonData.at("ChinaMarket").at("DayLineServer"); // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
		if (sTemp == _T("netease")) {
			m_iChinaMarketDayLineServer = 0;
		}
		else if (sTemp == _T("tengxun")) {
			m_iChinaMarketDayLineServer = 1;
		}
		else {// �Ƿ����������ƣ�ʹ��Ĭ��sina������
			m_iChinaMarketDayLineServer = 0;
			m_fUpdate = true;
		}
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iChinaMarketRTDataInquiryTime = jsonData.at("ChinaMarket").at("RealtimeInquiryTime"); // ʵʱ���ݲ�ѯʱ��������λ�����룩
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iSavingChinaMarketStockDayLineThread = jsonData.at("ChinaMarket").at("SavingStockDayLineThread"); // �����Ʊ���������߳�����
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iNumberOfRTDataSource = jsonData.at("ChinaMarket").at("NumberOfRTDataSource"); // Sinaʵʱ��������������
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
		m_iSinaRTDataInquiryPerTime = jsonData.at("ChinaMarket").at("SinaRTDataInquiryPerTime"); // Sinaʵʱ����ÿ�β�ѯ��Ʊ��
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iNeteaseRTDataInquiryPerTime = jsonData.at("ChinaMarket").at("NeteaseRTDataInquiryPerTime"); // Sinaʵʱ����ÿ�β�ѯ��Ʊ��
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iTengxunRTDataInquiryPerTime = jsonData.at("ChinaMarket").at("TengxunRTDataInquiryPerTime"); // Sinaʵʱ����ÿ�β�ѯ��Ʊ��
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}

	// WorldMarket
	try {
		m_lMarketResettingTime = jsonData.at("WorldMarket").at("MarketResettingTime"); // �г�����ʱ��
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
		m_iWorldMarketFinnhubInquiryTime = jsonData.at("WorldMarket").at("FinnhubInquiryTime"); // Ĭ��ÿСʱ����ѯ3000��
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iWorldMarketTiingoInquiryTime = jsonData.at("WorldMarket").at("TiingoInquiryTime"); // Ĭ��ÿСʱ����ѯ400��
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_iWorldMarketQuandlInquiryTime = jsonData.at("WorldMarket").at("QuandlInquiryTime"); // Ĭ��ÿСʱ����ѯ100��
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}

	// WebSocket
	try {
		m_bUsingFinnhubWebSocket = jsonData.at("WebSocket").at("UsingFinnhubWebSocket"); // �Ƿ�ʹ��Finnhub��WebSocket
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_bUsingTiingoIEXWebSocket = jsonData.at("WebSocket").at("UsingTiingoIEXWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_bUsingTiingoCryptoWebSocket = jsonData.at("WebSocket").at("UsingTiingoCryptoWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
	try {
		m_bUsingTiingoForexWebSocket = jsonData.at("WebSocket").at("UsingTiingoForexWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
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

	// ����ϵͳѡ��
	try {
		sTemp = jsonData.at("TestConfiguration").at("BenchmarkTestFileDirectory");
		m_strBenchmarkTestFileDirectory = sTemp.c_str();
	}
	catch (json::out_of_range&) {
		m_fUpdate = true;
	}
}

void CSystemConfiguration::UpdateJsonData(json& jsonData) {
	jsonData.clear(); // ���֮ǰ�����ݡ�
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

	// ����ϵͳѡ��
	jsonData["TestConfiguration"]["BenchmarkTestFileDirectory"] = m_strBenchmarkTestFileDirectory;
}

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

bool CSystemConfiguration::IsWebBusy() {
	return gl_pSinaRTDataSource->IsWebError() || gl_pNeteaseRTDataSource->IsWebError();
}

bool CSystemConfiguration::LoadDB() {
	fstream f(GetConfigurationFileDirectoryAndName(), ios::in);
	if (f.is_open()) {
		json systemConfiguration;
		f >> systemConfiguration;
		//systemConfiguration = json::parse(f); // ���ַ�ʽ�ȼ��� f >> m_systemConfiguration;
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
