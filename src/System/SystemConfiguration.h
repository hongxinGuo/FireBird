//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���е�ȫ�ֲ�����λ�ڴ��ļ��У�����ϵͳ���á�
//
// �洢��ʽʹ��json��ʽ���洢���ļ�SystemConfiguration.json�С�
//
// ʹ��Nlohmann��JSON��������֮��ʵ�����л��ͷ����л���
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"nlohmannJsonDeclaration.h" // ����˳�����json������ʹ�ô�ordered_json,�Ա�֤����������������ǰ��˳��һ�¡�

#include "NeteaseRTDataSource.h"
#include "SinaRTDataSource.h"

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
	void SaveDB() const;
	void Update();

	void ClearJson();
	void UpdateJson();

	void UpdateSystem() const; // ʹ�ñ�����ȥ����ϵͳ

	void ChangeFinnhubAccountTypeToFree();
	void ChangeFinnhubAccountTypeToPaid();
	bool IsPaidTypeFinnhubAccount() const noexcept { return m_bFinnhubAccountFeePaid; }

	void SetConfigurationFileDirectory(const CString& fileDirectory) { m_strDirectory = fileDirectory; }
	[[nodiscard]] CString GetConfigurationFileDirectory() { return m_strDirectory; }
	void SetDefaultFileName(const CString& fileName) { m_strFileName = fileName; }
	[[nodiscard]] CString GetDefaultFileName() { return m_strFileName; }
	[[nodiscard]] CString GetConfigurationFileDirectoryAndName() const { return m_strDirectory + m_strFileName; }

	// ��ȡϵͳ���ò���
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
		m_fUpdate = true;
		m_iChinaMarketRealtimeServer = iChinaMarketRealtimeServer;
	}

	[[nodiscard]] int GetChinaMarketDayLineServer() const noexcept { return m_iChinaMarketDayLineServer; }

	void SetChinaMarketDayLineServer(const int iChinaMarketDayLineServer) noexcept {
		m_fUpdate = true;
		m_iChinaMarketDayLineServer = iChinaMarketDayLineServer;
	}

	[[nodiscard]] int GetChinaMarketRTDataInquiryTime() const noexcept { return m_iChinaMarketRTDataInquiryTime; }

	void SetChinaMarketRTDataInquiryTime(const int iChinaMarketRTDataInquiryTime) noexcept {
		m_fUpdate = true;
		m_iChinaMarketRTDataInquiryTime = iChinaMarketRTDataInquiryTime;
	}

	[[nodiscard]] CString GetFinnhubToken() noexcept { return m_strFinnhubToken; }
	[[nodiscard]] CString GetTiingoToken() noexcept { return m_strTiingoToken; }
	[[nodiscard]] CString GetQuandlToken() noexcept { return m_strQuandlToken; }
	[[nodiscard]] int GetWorldMarketFinnhubInquiryTime() const noexcept { return m_iWorldMarketFinnhubInquiryTime; } // ��λΪ����
	void SetWorldMarketFinnhubInquiryTime(const int iWorldMarketFinnhubInquiryTime) noexcept {
		m_fUpdate = true;
		m_iWorldMarketFinnhubInquiryTime = iWorldMarketFinnhubInquiryTime;
	}

	[[nodiscard]] int GetWorldMarketTiingoInquiryTime() const noexcept { return m_iWorldMarketTiingoInquiryTime; }

	void SetWorldMarketTiingoInquiryTime(const int iWorldMarketTiingoInquiryTime) noexcept {
		m_fUpdate = true;
		m_iWorldMarketTiingoInquiryTime = iWorldMarketTiingoInquiryTime;
	}

	[[nodiscard]] int GetWorldMarketQuandlInquiryTime() const noexcept { return m_iWorldMarketQuandlInquiryTime; }

	void SetWorldMarketQuandlInquiryTime(const int iWorldMarketQuandlInquiryTime) noexcept {
		m_fUpdate = true;
		m_iWorldMarketQuandlInquiryTime = iWorldMarketQuandlInquiryTime;
	}

	[[nodiscard]] int GetRTServer() const { return m_iChinaMarketRealtimeServer; }
	[[nodiscard]] bool IsUsingSinaRTServer() const noexcept {
		if (m_iChinaMarketRealtimeServer == 0) return true;
		return false;
	}
	[[nodiscard]] bool IsUsingNeteaseRTServer() const noexcept {
		if (m_iChinaMarketRealtimeServer == 1) return true;
		return false;
	}
	[[nodiscard]] bool IsUsingTengxunRTServer() const noexcept {
		if (m_iChinaMarketRealtimeServer == 2) return true;
		return false;
	}

	[[nodiscard]] bool IsUsingNeteaseDayLineServer() const noexcept {
		if (m_iChinaMarketDayLineServer == 0) return true;
		return false;
	}

	[[nodiscard]] bool IsUsingTengxunDayLineServer() const noexcept {
		if (m_iChinaMarketDayLineServer == 1) return true;
		return false;
	}

	// WebSocket
	[[nodiscard]] bool IsUsingFinnhubWebSocket() const noexcept { return m_bUsingFinnhubWebSocket; }
	void SetUsingFinnhubWebSocket(const bool bUsingFinnhubWebSocket) noexcept { m_bUsingFinnhubWebSocket = bUsingFinnhubWebSocket; }
	[[nodiscard]] bool IsUsingTiingoIEXWebSocket() const noexcept { return m_bUsingTiingoIEXWebSocket; }

	void SetUsingTiingoIEXWebSocket(const bool bUsingTiingoIEXWebSocket) noexcept {
		m_fUpdate = true;
		m_bUsingTiingoIEXWebSocket = bUsingTiingoIEXWebSocket;
	}

	[[nodiscard]] bool IsUsingTiingoCryptoWebSocket() const noexcept { return m_bUsingTiingoCryptoWebSocket; }

	void SetUsingTiingoCryptoWebSocket(const bool bUsingTiingoCryptoWebSocket) noexcept {
		m_fUpdate = true;
		m_bUsingTiingoCryptoWebSocket = bUsingTiingoCryptoWebSocket;
	}

	[[nodiscard]] bool IsUsingTiingoForexWebSocket() const noexcept { return m_bUsingTiingoForexWebSocket; }

	void SetUsingTiingoForexWebSocket(const bool bUsingTiingoForexWebSocket) noexcept {
		m_fUpdate = true;
		m_bUsingTiingoForexWebSocket = bUsingTiingoForexWebSocket;
	}

	[[nodiscard]] bool IsUsingQuandlWebSocket() const noexcept { return m_bUsingQuandlWebSocket; }

	void SetUsingQuandlWebSocket(const bool bUsingQuandlWebSocket) noexcept {
		m_fUpdate = true;
		m_bUsingQuandlWebSocket = bUsingQuandlWebSocket;
	}

	// ChinaMarket
	[[nodiscard]] int GetSavingChinaMarketStockDayLineThread() const noexcept { return m_iSavingChinaMarketStockDayLineThread; }

	void SetSavingChinaMarketStockDayLineThread(const int iSavingChinaMarketStockDayLineThread) noexcept {
		m_fUpdate = true;
		m_iSavingChinaMarketStockDayLineThread = iSavingChinaMarketStockDayLineThread;
	}

	[[nodiscard]] bool IsFastInquiringRTData() const noexcept { return m_bFastInquiringRTData; }

	void SetFastInquiringRTData(const bool fFlag) noexcept {
		m_fUpdate = true;
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
		m_fUpdate = true;
		m_iStockProfileUpdateRate = iRate;
	}

	[[nodiscard]] int GetStockBasicFinancialUpdateRate() const noexcept { return m_iStockBasicFinancialUpdateRate; }

	void SetStockBasicFinancialUpdateRate(const int iRate) noexcept {
		m_fUpdate = true;
		m_iStockBasicFinancialUpdateRate = iRate;
	}

	[[nodiscard]] int GetInsideTransactionUpdateRate() const noexcept { return m_iInsideTransactionUpdateRate; }

	void SetInsideTransactionUpdateRate(const int iRate) noexcept {
		m_fUpdate = true;
		m_iInsideTransactionUpdateRate = iRate;
	}

	[[nodiscard]] int GetInsideSentimentUpdateRate() const noexcept { return m_iInsideSentimentUpdateRate; }

	void SetInsideSentimentUpdateRate(const int iRate) noexcept {
		m_fUpdate = true;
		m_iInsideSentimentUpdateRate = iRate;
	}

	[[nodiscard]] int GetStockPeerUpdateRate() const noexcept { return m_iStockPeerUpdateRate; }

	void SetStockPeerUpdateRate(const int iRate) noexcept {
		m_fUpdate = true;
		m_iStockPeerUpdateRate = iRate;
	}

	[[nodiscard]] int GetEPSSurpriseUpdateRate() const noexcept { return m_iEPSSurpriseUpdateRate; }

	void SetEPSSurpriseUpdateRate(const int iRate) noexcept {
		m_fUpdate = true;
		m_iEPSSurpriseUpdateRate = iRate;
	}

	[[nodiscard]] CString GetBenchmarkTestFileDirectory() const noexcept { return m_strBenchmarkTestFileDirectory; }

	void SetBenchmarkTestFileDirectory(const CString& strFileDirectory) {
		m_fUpdate = true;
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

	bool IsNeedUpdate() const noexcept { return m_fUpdate; }
	void SetUpdate(bool fFlag) noexcept { m_fUpdate = fFlag; }

	// ����洢
	void SetExitingSystem(const bool bExit) { m_fExitingSystem = bExit; }
	bool IsExitingSystem() { return m_fExitingSystem; }
	void SetExitingCalculatingRS(const bool bExit) { m_fExitingCalculatingRS = bExit; }
	bool IsExitingCalculatingRS() const { return m_fExitingCalculatingRS; }
	void SetWorkingMode(const bool bNormal) { m_fWorkingMode = bNormal; }
	bool IsWorkingMode() const { return m_fWorkingMode; }

	// ���������˻�������ʵʱ���ݳ�������ʱ��ϵͳ����������Ӧ���ò���
	static bool IsWebBusy() { return gl_pSinaRTDataSource->IsWebError() || gl_pNeteaseRTDataSource->IsWebError(); }

	bool IsInitialized() { return sm_bInitialized; }

protected:
	static bool sm_bInitialized; // ʹ�þ�̬��������ֻ֤����Ψһʵ�С�
	CString m_strDirectory; // �����ļ�Ŀ¼
	CString m_strFileName; // �����ļ�����

	// ϵͳ����
	bool m_bDebugMode; // ϵͳ�Ƿ��ǲ���״̬
	bool m_bReloadSystem; // ϵͳ�Ƿ���������������
	CString m_strDatabaseAccountName; // ���ݿ��˻�����
	CString m_strDatabaseAccountPassword; // ���ݿ��˻�����
	int m_iBackgroundThreadPermittedNumber; // ��̨�߳��������ֵ

	// ϵͳ����
	int m_iChinaMarketRealtimeServer; // �й��г�ʵʱ���ݷ�����.0:����ʵʱ���ݷ������� 1:����ʵʱ���ݷ�������
	int m_iChinaMarketDayLineServer; // �й��г��������ݷ�������0:�������߷�������1:��Ѷ���߷�������
	int m_iChinaMarketRTDataInquiryTime; // �й��г�ʵʱ���ݲ�ѯ���ʱ��

	// World Market
	CString m_strFinnhubToken; // ����
	CString m_strTiingoToken; // ����
	CString m_strQuandlToken; // ����
	bool m_bFinnhubAccountFeePaid; // �����˻���������˻�
	bool m_bTiingoAccountFeePaid;
	bool m_bQuandlAccountFeePaid;
	int m_iWorldMarketFinnhubInquiryTime; // ÿ�β�ѯ���ʱ�䣬��λΪ���롣�����˻�ÿ����300�Σ�ʵʱ����Ϊ900�Σ�������˻�ÿ����60�Ρ�
	int m_iWorldMarketTiingoInquiryTime; // ÿ�β�ѯ���ʱ�䣬��λΪ���롣�����˻�ÿСʱ5000�Σ�����˻�ÿСʱ50�Ρ�
	int m_iWorldMarketQuandlInquiryTime; // ÿ�β�ѯ���ʱ�䣬��λΪ����

	// WebSocket
	bool m_bUsingFinnhubWebSocket;
	bool m_bUsingTiingoIEXWebSocket;
	bool m_bUsingTiingoCryptoWebSocket;
	bool m_bUsingTiingoForexWebSocket;
	bool m_bUsingQuandlWebSocket;

	// ChinaMarket
	int m_iSavingChinaMarketStockDayLineThread; // �洢������ʷ����ʱ�Ĳ����߳�����
	bool m_bFastInquiringRTData;
	int m_iNumberOfRTDataSource; // ʵʱ���������������� Ĭ��Ϊ4
	int m_iSinaRTDataInquiryPerTime; // ����ʵʱ����ÿ�β�ѯ��Ʊ��
	int m_iNeteaseRTDataInquiryPerTime; // ����ʵʱ����ÿ�β�ѯ��Ʊ��
	int m_iTengxunRTDataInquiryPerTime; // ��Ѷʵʱ����ÿ�β�ѯ��Ʊ��

	// �������ݸ���Ƶ�ʣ��������ǣ�
	int m_iStockProfileUpdateRate; // Ĭ��45�����һ��
	int m_iInsideTransactionUpdateRate; // Ĭ��45�����һ��
	int m_iInsideSentimentUpdateRate; // Ĭ��45�����һ��
	int m_iStockBasicFinancialUpdateRate; // Ĭ��45�����һ��
	int m_iStockPeerUpdateRate; // Ĭ��90�����һ��
	int m_iEPSSurpriseUpdateRate; // Ĭ��90�����һ��

	// ����ϵͳ
	CString m_strBenchmarkTestFileDirectory; // ���ܲ����ļ����ڵ�Ŀ¼

	bool m_fUpdate;
	bool m_fInitialized = false;

	// ����Ϊ����洢֮ϵͳ����
	//
	// ���幤��������Ĳ���
	RECT m_rSystemDisplay; // ��ʾ��λ�����
	RECT m_rCurrentWindow; // ��ǰ����λ�����

	json m_systemConfiguration;

	//����洢�ı���
	std::atomic_bool m_fExitingSystem{false}; //  ϵͳ�˳���ʶ��������ֹ�����̡߳�
	bool m_fExitingCalculatingRS{false}; // ����֪ͨ�����߳��˳����ź�
	bool m_fWorkingMode{false}; // ����ģʽ��ʶ��Ĭ��Ϊ�١�ϵͳ��Ҫ������ʱ���ô˱�ʶ������ֻ�ж�ȡ���ݿ��Ȩ�����޷���Ӻ͸��ġ�
};

extern std::string gl_sSystemConfiguration; // ϵͳ������Ϣ

extern CSystemConfiguration gl_systemConfiguration;
