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

class CSystemConfiguration final : CObject {
public:
	CSystemConfiguration();
	// ֻ����һ��ʵ��,������ֵ��
	CSystemConfiguration(const CSystemConfiguration&) = delete;
	CSystemConfiguration& operator=(const CSystemConfiguration&) = delete;
	CSystemConfiguration(const CSystemConfiguration&&) noexcept = delete;
	CSystemConfiguration& operator=(const CSystemConfiguration&&) noexcept = delete;
	~CSystemConfiguration() override;

	bool LoadDB();
	bool SaveDB();
	void Update(void);
	void UpdateJson(void);

	void UpdateSystem(void); // ʹ�ñ�����ȥ����ϵͳ

	void SetDefaultFileDirectory(const CString& fileDirectory) { m_strDirectory = fileDirectory; }
	[[nodiscard]] CString GetDefaultFileDirectory(void) { return m_strDirectory; }

	void SetDefaultFileName(const CString& fileName) { m_strFileName = fileName; }
	[[nodiscard]] CString GetDefaultFileName(void) { return m_strFileName; }
	[[nodiscard]] CString GetDefaultFileDirectoryAndName(void) const { return m_strDirectory + m_strFileName; }

	// ��ȡϵͳ���ò���
	[[nodiscard]] bool IsUsingFastCPU(void) const noexcept { return m_bUsingFastCPU; }
	void SetUsingFastCPU(const bool bUsingFastCPU) noexcept { m_bUsingFastCPU = bUsingFastCPU; }
	[[nodiscard]] bool IsDebugMode(void) const noexcept { return m_bDebugMode; }
	void SetDebugMode(const bool bFlag) noexcept { m_bDebugMode = bFlag; }
	[[nodiscard]] CString GetDatabaseAccountName(void) noexcept { return m_strDatabaseAccountName; }
	void SetDatabaseAccountName(const CString& strAccountName) noexcept { m_strDatabaseAccountName = strAccountName; }
	[[nodiscard]] CString GetDatabaseAccountPassword(void) noexcept { return m_strDatabaseAccountPassword; }
	void SetDatabaseAccountPassword(const CString& strPassword) noexcept { m_strDatabaseAccountPassword = strPassword; }

	[[nodiscard]] int GetBackgroundThreadPermittedNumber(void) const noexcept { return m_iBackgroundThreadPermittedNumber; }
	[[nodiscard]] int GetSavingThreadPermittedNumber(void) const noexcept { return m_iSavingThreadPermittedNumber; }

	// ȫ�ֲ���
	[[nodiscard]] int GetChinaMarketRealtimeServer(void) const noexcept { return m_iChinaMarketRealtimeServer; }

	void SetChinaMarketRealtimeServer(const int iChinaMarketRealtimeServer) noexcept {
		m_fUpdate = true;
		m_iChinaMarketRealtimeServer = iChinaMarketRealtimeServer;
	}

	[[nodiscard]] int GetChinaMarketRTDataInquiryTime(void) const noexcept { return m_iChinaMarketRTDataInquiryTime; }

	void SetChinaMarketRTDataInquiryTime(const int iChinaMarketRTDataInquiryTime) noexcept {
		m_fUpdate = true;
		m_iChinaMarketRTDataInquiryTime = iChinaMarketRTDataInquiryTime;
	}

	[[nodiscard]] CString GetFinnhubToken(void) noexcept { return m_strFinnhubToken; }
	[[nodiscard]] CString GetTiingoToken(void) noexcept { return m_strTiingoToken; }
	[[nodiscard]] CString GetQuandlToken(void) noexcept { return m_strQuandlToken; }
	[[nodiscard]] int GetWorldMarketFinnhubInquiryTime(void) const noexcept { return m_iWorldMarketFinnhubInquiryTime; } // ��λΪ����
	void SetWorldMarketFinnhubInquiryTime(const int iWorldMarketFinnhubInquiryTime) noexcept {
		m_fUpdate = true;
		m_iWorldMarketFinnhubInquiryTime = iWorldMarketFinnhubInquiryTime;
	}

	[[nodiscard]] int GetWorldMarketTiingoInquiryTime(void) const noexcept { return m_iWorldMarketTiingoInquiryTime; }

	void SetWorldMarketTiingoInquiryTime(const int iWorldMarketTiingoInquiryTime) noexcept {
		m_fUpdate = true;
		m_iWorldMarketTiingoInquiryTime = iWorldMarketTiingoInquiryTime;
	}

	[[nodiscard]] int GetWorldMarketQuandlInquiryTime(void) const noexcept { return m_iWorldMarketQuandlInquiryTime; }

	void SetWorldMarketQuandlInquiryTime(const int iWorldMarketQuandlInquiryTime) noexcept {
		m_fUpdate = true;
		m_iWorldMarketQuandlInquiryTime = iWorldMarketQuandlInquiryTime;
	}

	[[nodiscard]] bool IsUsingSinaRTServer(void) const noexcept {
		if (m_iChinaMarketRealtimeServer == 0) return true;
		else return false;
	}

	[[nodiscard]] bool IsUsingNeteaseRTServer(void) const noexcept {
		if (m_iChinaMarketRealtimeServer == 1) return true;
		else return false;
	}

	// WebSocket
	[[nodiscard]] bool IsUsingFinnhubWebSocket(void) const noexcept { return m_bUsingFinnhubWebSocket; }
	void SetUsingFinnhubWebSocket(const bool bUsingFinnhubWebSocket) noexcept { m_bUsingFinnhubWebSocket = bUsingFinnhubWebSocket; }
	[[nodiscard]] bool IsUsingTiingoIEXWebSocket(void) const noexcept { return m_bUsingTiingoIEXWebSocket; }

	void SetUsingTiingoIEXWebSocket(const bool bUsingTiingoIEXWebSocket) noexcept {
		m_fUpdate = true;
		m_bUsingTiingoIEXWebSocket = bUsingTiingoIEXWebSocket;
	}

	[[nodiscard]] bool IsUsingTiingoCryptoWebSocket(void) const noexcept { return m_bUsingTiingoCryptoWebSocket; }

	void SetUsingTiingoCryptoWebSocket(const bool bUsingTiingoCryptoWebSocket) noexcept {
		m_fUpdate = true;
		m_bUsingTiingoCryptoWebSocket = bUsingTiingoCryptoWebSocket;
	}

	[[nodiscard]] bool IsUsingTiingoForexWebSocket(void) const noexcept { return m_bUsingTiingoForexWebSocket; }

	void SetUsingTiingoForexWebSocket(const bool bUsingTiingoForexWebSocket) noexcept {
		m_fUpdate = true;
		m_bUsingTiingoForexWebSocket = bUsingTiingoForexWebSocket;
	}

	[[nodiscard]] bool IsUsingQuandlWebSocket(void) const noexcept { return m_bUsingQuandlWebSocket; }

	void SetUsingQuandlWebSocket(const bool bUsingQuandlWebSocket) noexcept {
		m_fUpdate = true;
		m_bUsingQuandlWebSocket = bUsingQuandlWebSocket;
	}

	// ChinaMarket
	[[nodiscard]] int GetSavingChinaMarketStockDayLineThread(void) const noexcept { return m_iSavingChinaMarketStockDayLineThread; }

	void SetSavingChinaMarketStockDayLineThread(const int iSavingChinaMarketStockDayLineThread) noexcept {
		m_fUpdate = true;
		m_iSavingChinaMarketStockDayLineThread = iSavingChinaMarketStockDayLineThread;
	}

	[[nodiscard]] bool IsFastInquiringRTData(void) const noexcept { return m_bFastInquiringRTData; }

	void SetFastInquiringRTData(const bool fFlag) noexcept {
		m_fUpdate = true;
		m_bFastInquiringRTData = fFlag;
	}

	[[nodiscard]] int GetSinaRTDataInquiryPerTime(void) const noexcept { return m_iSinaRTDataInquiryPerTime; }
	void SetSinaRTDataInquiryPerTime(const int iNumber) noexcept { m_iSinaRTDataInquiryPerTime = iNumber; }
	[[nodiscard]] int GetNeteaseRTDataInquiryPerTime(void) const noexcept { return m_iNeteaseRTDataInquiryPerTime; }
	void SetNeteaseRTDataInquiryPerTime(const int iNumber) noexcept { m_iNeteaseRTDataInquiryPerTime = iNumber; }
	[[nodiscard]] int GetTengxunRTDataInquiryPerTime(void) const noexcept { return m_iTengxunRTDataInquiryPerTime; }
	void SetTengxunRTDataInquiryPerTime(const int iNumber) noexcept { m_iTengxunRTDataInquiryPerTime = iNumber; }

	// data update rate
	[[nodiscard]] int GetStockProfileUpdateRate(void) const noexcept { return m_iStockProfileUpdateRate; }

	void SetStockProfileUpdateRate(const int iRate) noexcept {
		m_fUpdate = true;
		m_iStockProfileUpdateRate = iRate;
	}

	[[nodiscard]] int GetStockBasicFinancialUpdateRate(void) const noexcept { return m_iStockBasicFinancialUpdateRate; }

	void SetStockBasicFinancialUpdateRate(const int iRate) noexcept {
		m_fUpdate = true;
		m_iStockBasicFinancialUpdateRate = iRate;
	}

	[[nodiscard]] int GetInsideTransactionUpdateRate(void) const noexcept { return m_iInsideTransactionUpdateRate; }

	void SetInsideTransactionUpdateRate(const int iRate) noexcept {
		m_fUpdate = true;
		m_iInsideTransactionUpdateRate = iRate;
	}

	[[nodiscard]] int GetInsideSentimentUpdateRate(void) const noexcept { return m_iInsideSentimentUpdateRate; }

	void SetInsideSentimentUpdateRate(const int iRate) noexcept {
		m_fUpdate = true;
		m_iInsideSentimentUpdateRate = iRate;
	}

	[[nodiscard]] int GetStockPeerUpdateRate(void) const noexcept { return m_iStockPeerUpdateRate; }

	void SetStockPeerUpdateRate(const int iRate) noexcept {
		m_fUpdate = true;
		m_iStockPeerUpdateRate = iRate;
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

	[[nodiscard]] RECT GetSystemDisplayRect(void) const noexcept { return m_rSystemDisplay; }
	[[nodiscard]] int GetSystemDisplayHeight(void) const noexcept { return m_rSystemDisplay.bottom - m_rSystemDisplay.top; }
	[[nodiscard]] int GetSystemDisplayWidth(void) const noexcept { return m_rSystemDisplay.right - m_rSystemDisplay.left; }
	void SetCurrentWindowRect(const RECT rect) noexcept { m_rCurrentWindow = rect; }

	void SetCurrentWindowRect(const int cx, const int cy) noexcept {
		m_rCurrentWindow.left = 0;
		m_rCurrentWindow.right = cx;
		m_rCurrentWindow.top = 0;
		m_rCurrentWindow.bottom = cy;
	}

	[[nodiscard]] RECT GetCurrentWindowRect(void) const noexcept { return m_rCurrentWindow; }
	[[nodiscard]] int GetCurrentWindowHeight(void) const noexcept { return m_rCurrentWindow.bottom - m_rCurrentWindow.top; }
	[[nodiscard]] int GetCurrentWindowWidth(void) const noexcept { return m_rCurrentWindow.right - m_rCurrentWindow.left; }

	// ����ϵͳѡ��

	[[nodiscard]] bool IsInitialized(void) const noexcept { return m_fInitialized; }

public:

protected:
	CString m_strDirectory; // �����ļ�Ŀ¼
	CString m_strFileName; // �����ļ�����

	// ϵͳ����
	bool m_bUsingFastCPU; //  �Ƿ�ʹ�ÿ���CPU
	bool m_bDebugMode; // ϵͳ�Ƿ��ǲ���״̬
	CString m_strDatabaseAccountName; // ���ݿ��˻�����
	CString m_strDatabaseAccountPassword; // ���ݿ��˻�����
	int m_iBackgroundThreadPermittedNumber; // ��̨�߳��������ֵ
	int m_iSavingThreadPermittedNumber; // ��������洢�߳����� Ĭ��Ϊ4

	// ϵͳ����
	int m_iChinaMarketRealtimeServer; // �й��г�ʵʱ���ݷ�����
	int m_iChinaMarketRTDataInquiryTime; // �й��г�ʵʱ���ݲ�ѯ���ʱ��

	int m_iWorldMarketFinnhubInquiryTime; // ÿ�β�ѯ���ʱ�䣬��λΪ����
	int m_iWorldMarketTiingoInquiryTime; // ÿ�β�ѯ���ʱ�䣬��λΪ����
	int m_iWorldMarketQuandlInquiryTime; // ÿ�β�ѯ���ʱ�䣬��λΪ����

	CString m_strFinnhubToken;
	CString m_strTiingoToken;
	CString m_strQuandlToken;

	// WebSocket
	bool m_bUsingFinnhubWebSocket;
	bool m_bUsingTiingoIEXWebSocket;
	bool m_bUsingTiingoCryptoWebSocket;
	bool m_bUsingTiingoForexWebSocket;
	bool m_bUsingQuandlWebSocket;

	// ChinaMarket
	int m_iSavingChinaMarketStockDayLineThread; // �洢������ʷ����ʱ�Ĳ����߳�����
	bool m_bFastInquiringRTData;
	int m_iSinaRTDataInquiryPerTime; // ����ʵʱ����ÿ�β�ѯ��Ʊ��
	int m_iNeteaseRTDataInquiryPerTime; // ����ʵʱ����ÿ�β�ѯ��Ʊ��
	int m_iTengxunRTDataInquiryPerTime; // ��Ѷʵʱ����ÿ�β�ѯ��Ʊ��

	// �������ݸ���Ƶ�ʣ��������ǣ�
	int m_iStockProfileUpdateRate; // Ĭ��45�����һ��
	int m_iInsideTransactionUpdateRate; // Ĭ��45�����һ��
	int m_iInsideSentimentUpdateRate; // Ĭ��45�����һ��
	int m_iStockBasicFinancialUpdateRate; // Ĭ��45�����һ��
	int m_iStockPeerUpdateRate; // Ĭ��90�����һ��

	// ����ϵͳ
	CString m_strBenchmarkTestFileDirectory; // ���ܲ����ļ����ڵ�Ŀ¼

	bool m_fUpdate;
	bool m_fInitialized = false;

	// ����Ϊ����洢֮ϵͳ����
	//
	// ���幤��������Ĳ���
	RECT m_rSystemDisplay; // ��ʾ��λ�����
	RECT m_rCurrentWindow; // ��ǰ����λ�����

	// ���뽫json����������������nlohmann json��������ǰ�棬���¸�ֵ����Ŀǰԭ������VSϵͳbug����nholmann json��bug�����߱ؾ���һ����
	// ��VSϵͳ17.2.4��bug��17.2.5�Ѿ����������bug��
	json m_systemConfiguration;
};

extern std::string gl_sSystemConfiguration; // ϵͳ������Ϣ

extern CSystemConfiguration gl_systemConfiguration;
