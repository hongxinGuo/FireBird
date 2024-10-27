///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"SystemConfiguration.h"

#include "NeteaseRTDataSource.h"
#include "SinaRTDataSource.h"

//#include<string>
//#include<fstream>

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
	"TengxunRTDataInquiryPerTime" : 900,
	"CurrentStock" : "600026.SS"
},

"WorldMarket" : {
	"MarketResettingTime" : 170000,
	"FinnhubToken" : "bv985d748v6u0",
	"FinnhubAccountFeePaid" : true,
	"QuandlToken" : "aBMXMyo_N3pMb3ex",
	"QuandlAccountFeePaid" : false,
	"FinnhubInquiryTime" : 1100,
	"TiingoInquiryTime" : 9000,
	"QuandlInquiryTime" : 36000
},

"Tiingo" : {
	"IEXTopOfBookUpdateDate" : 19990101,
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

	char buffer[200];
	_getcwd(buffer, 200);
	m_strDirectory = buffer;
	m_strDirectory = m_strDirectory + _T("\\"); //
	m_strFileName = _T("SystemConfiguration.json"); // json file name

#ifdef DEBUG
	m_bFastInquiringRTData = false; // ���ڲ��ԡ�����Ҫ����ϵͳʵʱ���ݽ��ո���ʱ��DEBUG״̬ʱ����Ϊ�档Ĭ��Ϊ��
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
	const CString strOld = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("json");
	const CString strNew = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("bak");
	DeleteFile(GetConfigurationFileDirectory() + strNew);
	rename(GetConfigurationFileDirectory() + strOld, GetConfigurationFileDirectory() + strNew); // ���汸��

	SaveDB();
	SetUpdateDB(false);
}

void CSystemConfiguration::Update(json& jsonData) {
	string sTemp;
	CString str1, str2, str3;
	str1.LoadString(nullptr, IDS_PROPERTYVIEW_SYSTEM_STATUS); // Note ʹ��LoadStringʱ��������ȷ����hInstanceΪnullptr�����򱨴�
	str2.LoadString(nullptr, IDS_PROPERTYVIEW_CHINA_MARKET_REALTIME);
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
		else m_iDisplayPropertyPage = 0; // Ĭ��ʹ��
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}

	// ϵͳ����
	try {
		m_iLogLevel = jsonData.at("SystemConfiguration").at("LogLevel");
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}

	try {
		m_bDebugMode = jsonData.at("SystemConfiguration").at("DebugMode");
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_bReloadSystem = jsonData.at("SystemConfiguration").at("ReloadSystem");
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}

	try {
		sTemp = jsonData.at("SystemConfiguration").at("DatabaseAccountName");
		m_strDatabaseAccountName = sTemp.c_str();
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		sTemp = jsonData.at("SystemConfiguration").at("DatabaseAccountPassword");
		m_strDatabaseAccountPassword = sTemp.c_str();
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iBackgroundThreadPermittedNumber = jsonData.at("SystemConfiguration").at("BackgroundThreadPermittedNumber");
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
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
			m_fUpdateDB = true;
		}
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
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
			m_fUpdateDB = true;
		}
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iChinaMarketRTDataInquiryTime = jsonData.at("ChinaMarket").at("RealtimeInquiryTime"); // ʵʱ���ݲ�ѯʱ��������λ�����룩
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iSavingChinaMarketStockDayLineThread = jsonData.at("ChinaMarket").at("SavingStockDayLineThread"); // �����Ʊ���������߳�����
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iNumberOfRTDataSource = jsonData.at("ChinaMarket").at("NumberOfRTDataSource"); // Sinaʵʱ��������������
		if (m_iNumberOfRTDataSource > 4) {
			m_iNumberOfRTDataSource = 4;
			m_fUpdateDB = true;
		}
		if (m_iNumberOfRTDataSource < 1) {
			m_iNumberOfRTDataSource = 1;
			m_fUpdateDB = true;
		}
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iSinaRTDataInquiryPerTime = jsonData.at("ChinaMarket").at("SinaRTDataInquiryPerTime"); // Sinaʵʱ����ÿ�β�ѯ��Ʊ��
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iNeteaseRTDataInquiryPerTime = jsonData.at("ChinaMarket").at("NeteaseRTDataInquiryPerTime"); // Sinaʵʱ����ÿ�β�ѯ��Ʊ��
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iTengxunRTDataInquiryPerTime = jsonData.at("ChinaMarket").at("TengxunRTDataInquiryPerTime"); // Sinaʵʱ����ÿ�β�ѯ��Ʊ��
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		sTemp = jsonData.at("ChinaMarket").at("CurrentStock"); // ʵʱ���ݷ�����ѡ��.0:����ʵʱ���ݣ�1������ʵʱ���ݣ�2����Ѷʵʱ���ݣ�Ŀǰ��ʹ�ã���
		m_strCurrentStock = sTemp.c_str();
	} catch (json::out_of_range&) {
		m_strCurrentStock = _T("");
		m_fUpdateDB = true;
	}

	// WorldMarket
	try {
		m_lMarketResettingTime = jsonData.at("WorldMarket").at("MarketResettingTime"); // �г�����ʱ��
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		sTemp = jsonData.at("WorldMarket").at("FinnhubToken"); // Finnhub token
		m_strFinnhubToken = sTemp.c_str();
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		sTemp = jsonData.at("WorldMarket").at("QuandlToken"); // Quandl token
		m_strQuandlToken = sTemp.c_str();
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_bQuandlAccountFeePaid = jsonData.at("WorldMarket").at("QuandlAccountFeePaid");
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iWorldMarketFinnhubInquiryTime = jsonData.at("WorldMarket").at("FinnhubInquiryTime"); // Ĭ��ÿСʱ����ѯ3000��
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iWorldMarketTiingoInquiryTime = jsonData.at("WorldMarket").at("TiingoInquiryTime"); // Ĭ��ÿСʱ����ѯ400��
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iWorldMarketQuandlInquiryTime = jsonData.at("WorldMarket").at("QuandlInquiryTime"); // Ĭ��ÿСʱ����ѯ100��
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}

	// Tiingo.com
	try {
		m_bTiingoAccountFeePaid = jsonData.at("Tiingo").at("AccountFeePaid"); // 
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		sTemp = jsonData.at("Tiingo").at("Token"); //
		m_strTiingoToken = sTemp.c_str();
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_lTiingoIEXTopOfBookUpdateDate = jsonData.at("Tiingo").at("IEXTopOfBookUpdateDate"); // 
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iTiingoHourLyRequestLimit = jsonData.at("Tiingo").at("HourlyRequestLimit"); // 
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_lTiingoDailyRequestLimit = jsonData.at("Tiingo").at("DailyRequestLimit"); // 
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_llTiingoBandWidth = jsonData.at("Tiingo").at("BandWidth"); // 
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_llTiingoBandWidthLeft = jsonData.at("Tiingo").at("BandWidthLeft"); // 
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}

	// WebSocket
	try {
		m_bUsingFinnhubWebSocket = jsonData.at("WebSocket").at("UsingFinnhubWebSocket"); // �Ƿ�ʹ��Finnhub��WebSocket
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_bUsingTiingoIEXWebSocket = jsonData.at("WebSocket").at("UsingTiingoIEXWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_bUsingTiingoCryptoWebSocket = jsonData.at("WebSocket").at("UsingTiingoCryptoWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_bUsingTiingoForexWebSocket = jsonData.at("WebSocket").at("UsingTiingoForexWebSocket"); // �Ƿ�ʹ��Tiingo��WebSocket
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}

	// Financial Data Update Rate
	try {
		m_iStockBasicFinancialUpdateRate = jsonData.at("FinancialDataUpdateRate").at("StockBasicFinancial");
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iStockProfileUpdateRate = jsonData.at("FinancialDataUpdateRate").at("StockProfile");
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iInsideTransactionUpdateRate = jsonData.at("FinancialDataUpdateRate").at("InsideTransaction");
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iInsideSentimentUpdateRate = jsonData.at("FinancialDataUpdateRate").at("InsideSentiment");
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iStockPeerUpdateRate = jsonData.at("FinancialDataUpdateRate").at("StockPeer");
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iTiingoCompanyFinancialStateUpdateRate = jsonData.at("FinancialDataUpdateRate").at("TiingoCompanyFinancialState");
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}

	try {
		m_iEPSSurpriseUpdateRate = jsonData.at("FinancialDataUpdateRate").at("EPSSurprise");
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}
	try {
		m_iSECFilingsUpdateRate = jsonData.at("FinancialDataUpdateRate").at("SECFilings");
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
	}

	// ����ϵͳѡ��
	try {
		sTemp = jsonData.at("TestConfiguration").at("BenchmarkTestFileDirectory");
		m_strBenchmarkTestFileDirectory = sTemp.c_str();
	} catch (json::out_of_range&) {
		m_fUpdateDB = true;
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
		str.LoadString(nullptr, IDS_PROPERTYVIEW_CHINA_MARKET_REALTIME);
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
	jsonData["ChinaMarket"]["CurrentStock"] = m_strCurrentStock;

	// World market
	jsonData["WorldMarket"]["MarketResettingTime"] = m_lMarketResettingTime;
	jsonData["WorldMarket"]["FinnhubToken"] = m_strFinnhubToken;
	jsonData["WorldMarket"]["FinnhubAccountFeePaid"] = m_bFinnhubAccountFeePaid;
	jsonData["WorldMarket"]["QuandlToken"] = m_strQuandlToken;
	jsonData["WorldMarket"]["QuandlAccountFeePaid"] = m_bQuandlAccountFeePaid;
	jsonData["WorldMarket"]["FinnhubInquiryTime"] = m_iWorldMarketFinnhubInquiryTime;
	jsonData["WorldMarket"]["TiingoInquiryTime"] = m_iWorldMarketTiingoInquiryTime;
	jsonData["WorldMarket"]["QuandlInquiryTime"] = m_iWorldMarketQuandlInquiryTime;

	// Tiingo.com
	jsonData["Tiingo"]["AccountFeePaid"] = m_bTiingoAccountFeePaid;
	jsonData["Tiingo"]["Token"] = m_strTiingoToken;
	jsonData["Tiingo"]["IEXTopOfBookUpdateDate"] = m_lTiingoIEXTopOfBookUpdateDate;
	jsonData["Tiingo"]["HourlyRequestLimit"] = m_iTiingoHourLyRequestLimit;
	jsonData["Tiingo"]["DailyRequestLimit"] = m_lTiingoDailyRequestLimit;
	jsonData["Tiingo"]["BandWidth"] = m_llTiingoBandWidth;
	jsonData["Tiingo"]["BandWidthLeft"] = m_llTiingoBandWidthLeft;

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

	jsonData["FinancialDataUpdateRate"]["TiingoCompanyFinancialState"] = m_iTiingoCompanyFinancialStateUpdateRate;

	// ����ϵͳѡ��
	jsonData["TestConfiguration"]["BenchmarkTestFileDirectory"] = m_strBenchmarkTestFileDirectory;
}

void CSystemConfiguration::ChangeFinnhubAccountTypeToFree() {
	m_bFinnhubAccountFeePaid = false;
	m_iWorldMarketFinnhubInquiryTime = 1100; // ÿ��1100����
	m_fUpdateDB = true;
}

void CSystemConfiguration::ChangeFinnhubAccountTypeToPaid() {
	m_bFinnhubAccountFeePaid = true;
	m_iWorldMarketFinnhubInquiryTime = 220; // ÿ��220����
	m_fUpdateDB = true;
}

void CSystemConfiguration::ChangeTiingoAccountTypeToFree() {
	m_bTiingoAccountFeePaid = false;
	m_iWorldMarketTiingoInquiryTime = 9000; // ÿ��9000����
	m_fUpdateDB = true;
}

void CSystemConfiguration::ChangeTiingoAccountTypeToPaid() {
	m_bTiingoAccountFeePaid = true;
	m_iWorldMarketTiingoInquiryTime = 500; // ÿ��500����.ÿСʱ����޶�Ϊ10000��
	m_fUpdateDB = true;
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
