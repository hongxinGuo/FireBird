//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���е�ȫ�ֲ�����λ�ڴ��ļ��У�����ϵͳ���á�
//
// �洢��ʽʹ��json��ʽ���洢���ļ�SystemConfiguration.json�С�
//
// ʹ��Nlohmann��JSON��������֮��ʵ�����л��ͷ����л���
//
// todo ׼��ʹ��simdjson��װ��json���ݣ�Nlohmannjsonֻ���ڴ洢json���ݡ�
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"nlohmannJsonDeclaration.h" // ����˳�����json������ʹ�ô�ordered_json,�Ա�֤����������������ǰ��˳��һ�¡�
#include"nlohmann/json.hpp"

class CSystemConfiguration final {
public:
	CSystemConfiguration();
	// ֻ����һ��ʵ��,������ֵ��
	CSystemConfiguration(const CSystemConfiguration&) = delete;
	CSystemConfiguration& operator=(const CSystemConfiguration&) = delete;
	CSystemConfiguration(const CSystemConfiguration&&) noexcept = delete;
	CSystemConfiguration& operator=(const CSystemConfiguration&&) noexcept = delete;
	~CSystemConfiguration();

	void UpdateDB();
	bool LoadDB();
	void SaveDB();
	void Update(json& jsonData);
	void UpdateJsonData(json& jsonData);

	void ChangeFinnhubAccountTypeToFree();
	void ChangeFinnhubAccountTypeToPaid();
	bool IsPaidTypeFinnhubAccount() const noexcept { return m_bFinnhubAccountFeePaid; }

	void ChangeTiingoAccountTypeToFree();
	void ChangeTiingoAccountTypeToPaid();
	bool IsPaidTypeTiingoAccount() const noexcept { return m_bTiingoAccountFeePaid; }

	void SetConfigurationFileDirectory(const CString& fileDirectory) { m_strDirectory = fileDirectory; }
	[[nodiscard]] CString GetConfigurationFileDirectory() { return m_strDirectory; }
	void SetDefaultFileName(const CString& fileName) { m_strFileName = fileName; }
	[[nodiscard]] CString GetDefaultFileName() { return m_strFileName; }
	[[nodiscard]] CString GetConfigurationFileDirectoryAndName() const { return m_strDirectory + m_strFileName; }

	// ��������
	int GetDisplayPropertyPage() const noexcept { return m_iDisplayPropertyPage; }
	void SetDisplayPropertyPage(int page) noexcept { m_iDisplayPropertyPage = page; }

	// ��ȡϵͳ���ò���
	int GetLogLevel() const noexcept { return m_iLogLevel; }
	void SetLogLevel(int level) noexcept { m_iLogLevel = level; }
	[[nodiscard]] bool IsDebugMode() const noexcept { return m_bDebugMode; }
	void SetDebugMode(const bool bFlag) noexcept { m_bDebugMode = bFlag; }
	[[nodiscard]] bool IsReloadSystem() const noexcept { return m_bReloadSystem; }
	void SetReloadSystem(const bool bFlag) noexcept { m_bReloadSystem = bFlag; }
	[[nodiscard]] CString GetDatabaseAccountName() noexcept { return m_strDatabaseAccountName; }
	void SetDatabaseAccountName(const CString& strAccountName) noexcept { m_strDatabaseAccountName = strAccountName; }
	[[nodiscard]] CString GetDatabaseAccountPassword() noexcept { return m_strDatabaseAccountPassword; }
	void SetDatabaseAccountPassword(const CString& strPassword) noexcept { m_strDatabaseAccountPassword = strPassword; }

	[[nodiscard]] int GetBackgroundThreadPermittedNumber() const noexcept { return m_iBackgroundThreadPermittedNumber; }

	// ȫ�ֲ���
	[[nodiscard]] int GetChinaMarketRealtimeServer() const noexcept { return m_iChinaMarketRealtimeServer; }
	void UsingSinaRealtimeServer();
	void UsingNeteaseRealtimeServer();
	void UsingTengxunRealtimeServer();

	void SetChinaMarketRealtimeServer(const int iChinaMarketRealtimeServer) noexcept {
		m_fUpdateDB = true;
		m_iChinaMarketRealtimeServer = iChinaMarketRealtimeServer;
	}
	[[nodiscard]] int GetChinaMarketDayLineServer() const noexcept { return m_iChinaMarketDayLineServer; }

	void SetChinaMarketDayLineServer(const int iChinaMarketDayLineServer) noexcept {
		m_fUpdateDB = true;
		m_iChinaMarketDayLineServer = iChinaMarketDayLineServer;
	}
	[[nodiscard]] int GetChinaMarketRTDataInquiryTime() const noexcept { return m_iChinaMarketRTDataInquiryTime; }

	void SetChinaMarketRTDataInquiryTime(const int iChinaMarketRTDataInquiryTime) noexcept {
		m_fUpdateDB = true;
		m_iChinaMarketRTDataInquiryTime = iChinaMarketRTDataInquiryTime;
	}

	// World market
	long GetWorldMarketResettingTime() const noexcept { return m_lMarketResettingTime; }
	[[nodiscard]] CString GetFinnhubToken() noexcept { return m_strFinnhubToken; }
	[[nodiscard]] CString GetTiingoToken() noexcept { return m_strTiingoToken; }
	[[nodiscard]] CString GetQuandlToken() noexcept { return m_strQuandlToken; }
	[[nodiscard]] int GetWorldMarketFinnhubInquiryTime() const noexcept { return m_iWorldMarketFinnhubInquiryTime; } // ��λΪ����
	void SetWorldMarketFinnhubInquiryTime(const int iWorldMarketFinnhubInquiryTime) noexcept {
		m_fUpdateDB = true;
		m_iWorldMarketFinnhubInquiryTime = iWorldMarketFinnhubInquiryTime;
	}
	[[nodiscard]] int GetWorldMarketTiingoInquiryTime() const noexcept { return m_iWorldMarketTiingoInquiryTime; }
	void SetWorldMarketTiingoInquiryTime(const int iWorldMarketTiingoInquiryTime) noexcept {
		m_fUpdateDB = true;
		m_iWorldMarketTiingoInquiryTime = iWorldMarketTiingoInquiryTime;
	}
	[[nodiscard]] int GetWorldMarketQuandlInquiryTime() const noexcept { return m_iWorldMarketQuandlInquiryTime; }
	void SetWorldMarketQuandlInquiryTime(const int iWorldMarketQuandlInquiryTime) noexcept {
		m_fUpdateDB = true;
		m_iWorldMarketQuandlInquiryTime = iWorldMarketQuandlInquiryTime;
	}

	[[nodiscard]] int GetRTServer() const { return m_iChinaMarketRealtimeServer; }
	[[nodiscard]] bool IsUsingSinaRTServer() const noexcept { return m_iChinaMarketRealtimeServer == 0; }
	[[nodiscard]] bool IsUsingNeteaseRTServer() const noexcept { return m_iChinaMarketRealtimeServer == 1; }
	[[nodiscard]] bool IsUsingTengxunRTServer() const noexcept { return m_iChinaMarketRealtimeServer == 2; }
	[[nodiscard]] bool IsUsingNeteaseDayLineServer() const noexcept { return m_iChinaMarketDayLineServer == 0; }
	[[nodiscard]] bool IsUsingTengxunDayLineServer() const noexcept { return m_iChinaMarketDayLineServer == 1; }

	void SetCurrentStock(const CString& stock) noexcept { m_strCurrentStock = stock; }
	CString GetCurrentStock() const noexcept { return m_strCurrentStock; }

	// Tiingo.com
	void ChangeTiingoAccountTypeToFree2();
	void ChangeTiingoAccountTypeToPaid2();
	bool IsPaidTypeTiingoAccount2() const noexcept { return m_bTiingoAccountFeePaid2; }
	[[nodiscard]] CString GetTiingoToken2() noexcept { return m_strTiingoToken2; }

	[[nodiscard]] int GetTiingoHourLyRequestLimit() const noexcept { return m_iTiingoHourLyRequestLimit; }
	void SetTiingoHourLyRequestLimit(const int iTiingoHourLyRequestLimit) noexcept {
		m_fUpdateDB = true;
		m_iTiingoHourLyRequestLimit = iTiingoHourLyRequestLimit;
	}
	[[nodiscard]] long GetTiingoDailyRequestLimit() const noexcept { return m_lTiingoDailyRequestLimit; }
	void SetTiingoDailyRequestLimit(const long lTiingoDailyRequestLimit) noexcept {
		m_fUpdateDB = true;
		m_lTiingoDailyRequestLimit = lTiingoDailyRequestLimit;
	}
	[[nodiscard]] long long GetTiingoBandWidth() const noexcept { return m_llTiingoBandWidth; }
	void SetTiingoBandWidth(const long long llTiingoBandWidth) noexcept {
		m_fUpdateDB = true;
		m_llTiingoBandWidth = llTiingoBandWidth;
	}
	[[nodiscard]] long long GetTiingoBandWidthLeft() const noexcept { return m_llTiingoBandWidthLeft; }
	void DecreaseTiingoBandWidth(long long llSize) noexcept { m_llTiingoBandWidthLeft -= llSize; }
	void SetTiingoBandWidthLeft(const long long llTiingoBandWidthLeft) noexcept {
		m_fUpdateDB = true;
		m_llTiingoBandWidthLeft = llTiingoBandWidthLeft;
	}

	// WebSocket
	[[nodiscard]] bool IsUsingFinnhubWebSocket() const noexcept { return m_bUsingFinnhubWebSocket; }
	void SetUsingFinnhubWebSocket(const bool bUsingFinnhubWebSocket) noexcept { m_bUsingFinnhubWebSocket = bUsingFinnhubWebSocket; }

	[[nodiscard]] bool IsUsingTiingoIEXWebSocket() const noexcept { return m_bUsingTiingoIEXWebSocket; }
	void SetUsingTiingoIEXWebSocket(const bool bUsingTiingoIEXWebSocket) noexcept {
		m_fUpdateDB = true;
		m_bUsingTiingoIEXWebSocket = bUsingTiingoIEXWebSocket;
	}

	[[nodiscard]] bool IsUsingTiingoCryptoWebSocket() const noexcept { return m_bUsingTiingoCryptoWebSocket; }
	void SetUsingTiingoCryptoWebSocket(const bool bUsingTiingoCryptoWebSocket) noexcept {
		m_fUpdateDB = true;
		m_bUsingTiingoCryptoWebSocket = bUsingTiingoCryptoWebSocket;
	}

	[[nodiscard]] bool IsUsingTiingoForexWebSocket() const noexcept { return m_bUsingTiingoForexWebSocket; }
	void SetUsingTiingoForexWebSocket(const bool bUsingTiingoForexWebSocket) noexcept {
		m_fUpdateDB = true;
		m_bUsingTiingoForexWebSocket = bUsingTiingoForexWebSocket;
	}

	[[nodiscard]] bool IsUsingQuandlWebSocket() const noexcept { return m_bUsingQuandlWebSocket; }
	void SetUsingQuandlWebSocket(const bool bUsingQuandlWebSocket) noexcept {
		m_fUpdateDB = true;
		m_bUsingQuandlWebSocket = bUsingQuandlWebSocket;
	}

	// ChinaMarket
	[[nodiscard]] int GetSavingChinaMarketStockDayLineThread() const noexcept { return m_iSavingChinaMarketStockDayLineThread; }
	void SetSavingChinaMarketStockDayLineThread(const int iSavingChinaMarketStockDayLineThread) noexcept {
		m_fUpdateDB = true;
		m_iSavingChinaMarketStockDayLineThread = iSavingChinaMarketStockDayLineThread;
	}

	[[nodiscard]] bool IsFastInquiringRTData() const noexcept { return m_bFastInquiringRTData; }
	void SetFastInquiringRTData(const bool fFlag) noexcept {
		m_fUpdateDB = true;
		m_bFastInquiringRTData = fFlag;
	}

	[[nodiscard]] int GetNumberOfRTDataSource() const noexcept { return m_iNumberOfRTDataSource; }
	void SetNumberOfRTDataSource(const int iNumber) noexcept { m_iNumberOfRTDataSource = iNumber; }
	[[nodiscard]] int GetSinaRTDataInquiryPerTime() const noexcept { return m_iSinaRTDataInquiryPerTime; }
	void SetSinaRTDataInquiryPerTime(const int iNumber) noexcept { m_iSinaRTDataInquiryPerTime = iNumber; }
	[[nodiscard]] int GetNeteaseRTDataInquiryPerTime() const noexcept { return m_iNeteaseRTDataInquiryPerTime; }
	void SetNeteaseRTDataInquiryPerTime(const int iNumber) noexcept { m_iNeteaseRTDataInquiryPerTime = iNumber; }
	[[nodiscard]] int GetTengxunRTDataInquiryPerTime() const noexcept { return m_iTengxunRTDataInquiryPerTime; }
	void SetTengxunRTDataInquiryPerTime(const int iNumber) noexcept { m_iTengxunRTDataInquiryPerTime = iNumber; }

	// data update rate
	[[nodiscard]] int GetStockProfileUpdateRate() const noexcept { return m_iStockProfileUpdateRate; }
	void SetStockProfileUpdateRate(const int iRate) noexcept {
		m_fUpdateDB = true;
		m_iStockProfileUpdateRate = iRate;
	}

	[[nodiscard]] int GetStockBasicFinancialUpdateRate() const noexcept { return m_iStockBasicFinancialUpdateRate; }
	void SetStockBasicFinancialUpdateRate(const int iRate) noexcept {
		m_fUpdateDB = true;
		m_iStockBasicFinancialUpdateRate = iRate;
	}

	[[nodiscard]] int GetInsideTransactionUpdateRate() const noexcept { return m_iInsideTransactionUpdateRate; }
	void SetInsideTransactionUpdateRate(const int iRate) noexcept {
		m_fUpdateDB = true;
		m_iInsideTransactionUpdateRate = iRate;
	}

	[[nodiscard]] int GetInsideSentimentUpdateRate() const noexcept { return m_iInsideSentimentUpdateRate; }
	void SetInsideSentimentUpdateRate(const int iRate) noexcept {
		m_fUpdateDB = true;
		m_iInsideSentimentUpdateRate = iRate;
	}

	[[nodiscard]] int GetStockPeerUpdateRate() const noexcept { return m_iStockPeerUpdateRate; }
	void SetStockPeerUpdateRate(const int iRate) noexcept {
		m_fUpdateDB = true;
		m_iStockPeerUpdateRate = iRate;
	}

	[[nodiscard]] int GetEPSSurpriseUpdateRate() const noexcept { return m_iEPSSurpriseUpdateRate; }
	void SetEPSSurpriseUpdateRate(const int iRate) noexcept {
		m_fUpdateDB = true;
		m_iEPSSurpriseUpdateRate = iRate;
	}

	[[nodiscard]] int GetSECFilingsUpdateRate() const noexcept { return m_iSECFilingsUpdateRate; }
	void SetSECFilingsUpdateRate(const int iRate) noexcept {
		m_fUpdateDB = true;
		m_iSECFilingsUpdateRate = iRate;
	}

	[[nodiscard]] int GetTiingoCompanyFinancialStateUpdateRate() const noexcept { return m_iTiingoCompanyFinancialStateUpdateRate; }
	void SetTiingoCompanyFinancialStateUpdateRate(const int iRate) noexcept {
		m_fUpdateDB = true;
		m_iTiingoCompanyFinancialStateUpdateRate = iRate;
	}

	[[nodiscard]] CString GetBenchmarkTestFileDirectory() const noexcept { return m_strBenchmarkTestFileDirectory; }
	void SetBenchmarkTestFileDirectory(const CString& strFileDirectory) {
		m_fUpdateDB = true;
		m_strBenchmarkTestFileDirectory = strFileDirectory;
	}

	void SetSystemDisplayRect(const RECT rect) noexcept { m_rSystemDisplay = rect; }
	void SetSystemDisplayRect(const int cx, const int cy) noexcept {
		m_rSystemDisplay.left = 0;
		m_rSystemDisplay.right = cx;
		m_rSystemDisplay.top = 0;
		m_rSystemDisplay.bottom = cy;
	}

	[[nodiscard]] RECT GetSystemDisplayRect() const noexcept { return m_rSystemDisplay; }
	[[nodiscard]] int GetSystemDisplayHeight() const noexcept { return m_rSystemDisplay.bottom - m_rSystemDisplay.top; }
	[[nodiscard]] int GetSystemDisplayWidth() const noexcept { return m_rSystemDisplay.right - m_rSystemDisplay.left; }
	void SetCurrentWindowRect(const RECT rect) noexcept { m_rCurrentWindow = rect; }

	void SetCurrentWindowRect(const int cx, const int cy) noexcept {
		m_rCurrentWindow.left = 0;
		m_rCurrentWindow.right = cx;
		m_rCurrentWindow.top = 0;
		m_rCurrentWindow.bottom = cy;
	}

	[[nodiscard]] RECT GetCurrentWindowRect() const noexcept { return m_rCurrentWindow; }
	[[nodiscard]] int GetCurrentWindowHeight() const noexcept { return m_rCurrentWindow.bottom - m_rCurrentWindow.top; }
	[[nodiscard]] int GetCurrentWindowWidth() const noexcept { return m_rCurrentWindow.right - m_rCurrentWindow.left; }

	// ����ϵͳѡ��

	[[nodiscard]] bool IsInitialized() const noexcept { return m_fInitialized; }

	bool IsUpdateDB() const noexcept { return m_fUpdateDB; }
	void SetUpdateDB(bool fFlag) noexcept { m_fUpdateDB = fFlag; }

	// ����洢
	void SetExitingSystem(const bool bExit) { m_fExitingSystem = bExit; }
	bool IsExitingSystem() { return m_fExitingSystem; }
	void SetExitingCalculatingRS(const bool bExit) { m_fExitingCalculatingRS = bExit; }
	bool IsExitingCalculatingRS() const { return m_fExitingCalculatingRS; }
	void SetWorkingMode(const bool bNormal) { m_fWorkingMode = bNormal; }
	bool IsWorkingMode() const { return m_fWorkingMode; }

	// ���������˻�������ʵʱ���ݳ�������ʱ��ϵͳ����������Ӧ���ò���
	bool IsWebBusy();

	bool IsInitialized() { return sm_bInitialized; }

protected:
	static bool sm_bInitialized; // ʹ�þ�̬��������ֻ֤����Ψһʵ�С�
	CString m_strDirectory; // �����ļ�Ŀ¼
	CString m_strFileName; // �����ļ�����

	// ��������
	int m_iDisplayPropertyPage{ 0 }; // application : 0, 

	// ϵͳ����
	int m_iLogLevel; // spdlog��־�ļ���¼�ȼ�: trace, debug, info, warn, error, critical, off
	bool m_bDebugMode{ false }; // ϵͳ�Ƿ��ǲ���״̬
	bool m_bReloadSystem{ false }; // ϵͳ�Ƿ���������������
	CString m_strDatabaseAccountName; // ���ݿ��˻�����
	CString m_strDatabaseAccountPassword; // ���ݿ��˻�����
	int m_iBackgroundThreadPermittedNumber; // ��̨�߳��������ֵ

	// ϵͳ����
	int m_iChinaMarketRealtimeServer{ 0 }; // �й��г�ʵʱ���ݷ�����.0:����ʵʱ���ݷ������� 1:����ʵʱ���ݷ�������
	int m_iChinaMarketDayLineServer{ 0 }; // �й��г��������ݷ�������0:�������߷�������1:��Ѷ���߷�������
	int m_iChinaMarketRTDataInquiryTime{ 250 }; // �й��г�ʵʱ���ݲ�ѯ���ʱ��,��λΪ����

	// World Market
	long m_lMarketResettingTime{ 170000 };
	CString m_strFinnhubToken; // ����
	CString m_strTiingoToken; // ����
	CString m_strQuandlToken; // ����
	bool m_bFinnhubAccountFeePaid{ true }; // �����˻���������˻�
	bool m_bTiingoAccountFeePaid{ true };
	bool m_bQuandlAccountFeePaid{ true };
	int m_iWorldMarketFinnhubInquiryTime{ 1100 }; // ÿ�β�ѯ���ʱ�䣬��λΪ���롣�����˻�ÿ����300�Σ�ʵʱ����Ϊ900�Σ�������˻�ÿ����60�Ρ�
	int m_iWorldMarketTiingoInquiryTime{ 3600000 / 500 }; // ÿ�β�ѯ���ʱ�䣬��λΪ���롣�����˻�ÿСʱ5000�Σ�����˻�ÿСʱ50�Ρ�
	int m_iWorldMarketQuandlInquiryTime{ 3600000 / 100 }; // ÿ�β�ѯ���ʱ�䣬��λΪ����
	CString m_strCurrentStock{ _T("") }; // ��ǰ��ѡ��Ʊ

	// Tiingo.com
	bool m_bTiingoAccountFeePaid2{ true };
	CString m_strTiingoToken2{ _T("") };
	int m_iTiingoHourLyRequestLimit{ 500 };
	long m_lTiingoDailyRequestLimit{ 20000 };
	long long m_llTiingoBandWidth{ 5368709120 };
	long long m_llTiingoBandWidthLeft{ 5368709120 };

	// WebSocket
	bool m_bUsingFinnhubWebSocket{ true };
	bool m_bUsingTiingoIEXWebSocket{ true };
	bool m_bUsingTiingoCryptoWebSocket{ true };
	bool m_bUsingTiingoForexWebSocket{ true };
	bool m_bUsingQuandlWebSocket{ true };

	// ChinaMarket
	int m_iSavingChinaMarketStockDayLineThread{ 4 }; // �洢������ʷ����ʱ�Ĳ����߳�����
	bool m_bFastInquiringRTData{ false };
	int m_iNumberOfRTDataSource{ 4 }; // ʵʱ���������������� Ĭ��Ϊ4
	int m_iSinaRTDataInquiryPerTime{ 850 }; // ����ʵʱ����ÿ�β�ѯ��Ʊ��
	int m_iNeteaseRTDataInquiryPerTime{ 900 }; // ����ʵʱ����ÿ�β�ѯ��Ʊ��
	int m_iTengxunRTDataInquiryPerTime{ 900 }; // ��Ѷʵʱ����ÿ�β�ѯ��Ʊ��

	// �������ݸ���Ƶ�ʣ��������ǣ�
	// Finnhub����
	int m_iStockProfileUpdateRate{ 365 }; // Ĭ��365�����һ��
	int m_iInsideTransactionUpdateRate{ 30 }; // Ĭ��30�����һ��
	int m_iInsideSentimentUpdateRate{ 30 }; // Ĭ��30�����һ��
	int m_iStockBasicFinancialUpdateRate{ 45 }; // Ĭ��45�����һ��
	int m_iStockPeerUpdateRate{ 90 }; // Ĭ��90�����һ��
	int m_iEPSSurpriseUpdateRate{ 90 }; // Ĭ��90�����һ��
	int m_iSECFilingsUpdateRate{ 30 }; // Ĭ��30�����һ��
	// Tiingo ����
	int m_iTiingoCompanyFinancialStateUpdateRate{ 45 }; // Tiingo��˾�������ݸ���

	// ����ϵͳ
	CString m_strBenchmarkTestFileDirectory; // ���ܲ����ļ����ڵ�Ŀ¼

	bool m_fUpdateDB{ false };
	bool m_fInitialized{ false };

	// ����Ϊ����洢֮ϵͳ����
	//
	// ���幤��������Ĳ���
	RECT m_rSystemDisplay; // ��ʾ��λ�����
	RECT m_rCurrentWindow; // ��ǰ����λ�����

	//����洢�ı���
	std::atomic_bool m_fExitingSystem{ false }; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
	bool m_fExitingCalculatingRS{ false }; // ����֪ͨ�����߳��˳����ź�
	bool m_fWorkingMode{ true }; // ����ģʽ��ʶ��Ĭ��Ϊ�١�ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
};

extern std::string gl_sSystemConfiguration; // ϵͳ������Ϣ

extern CSystemConfiguration gl_systemConfiguration;
