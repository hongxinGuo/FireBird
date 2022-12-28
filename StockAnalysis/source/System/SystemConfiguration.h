//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 所有的全局参数皆位于此文件中，用于系统配置。
//
// 存储方式使用json格式，存储于文件SystemConfiguration.json中。
//
// 使用Nlohmann的JSON库来解析之，实现序列化和反序列化。
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"nlohmannJsonDeclaration.h" // 按照顺序输出json，必须使用此ordered_json,以保证解析后的数据与解析前的顺序一致。

class CSystemConfiguration final : CObject {
public:
	CSystemConfiguration();
	// 只能有一个实例,不允许赋值。
	CSystemConfiguration(const CSystemConfiguration&) = delete;
	CSystemConfiguration& operator=(const CSystemConfiguration&) = delete;
	CSystemConfiguration(const CSystemConfiguration&&) noexcept = delete;
	CSystemConfiguration& operator=(const CSystemConfiguration&&) noexcept = delete;
	~CSystemConfiguration() override;

	bool LoadDB();
	bool SaveDB();
	void Update(void);
	void UpdateJson(void);

	void UpdateSystem(void); // 使用本参数去更新系统

	void SetDefaultFileDirectory(const CString& fileDirectory) { m_strDirectory = fileDirectory; }
	[[nodiscard]] CString GetDefaultFileDirectory(void) { return m_strDirectory; }

	void SetDefaultFileName(const CString& fileName) { m_strFileName = fileName; }
	[[nodiscard]] CString GetDefaultFileName(void) { return m_strFileName; }
	[[nodiscard]] CString GetDefaultFileDirectoryAndName(void) const { return m_strDirectory + m_strFileName; }

	// 获取系统配置参数
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

	// 全局参数
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
	[[nodiscard]] int GetWorldMarketFinnhubInquiryTime(void) const noexcept { return m_iWorldMarketFinnhubInquiryTime; } // 单位为毫秒
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

	// 测试系统选项

	[[nodiscard]] bool IsInitialized(void) const noexcept { return m_fInitialized; }

public:

protected:
	CString m_strDirectory; // 配置文件目录
	CString m_strFileName; // 配置文件名称

	// 系统配置
	bool m_bUsingFastCPU; //  是否使用快速CPU
	bool m_bDebugMode; // 系统是否是测试状态
	CString m_strDatabaseAccountName; // 数据库账户名称
	CString m_strDatabaseAccountPassword; // 数据库账户密码
	int m_iBackgroundThreadPermittedNumber; // 后台线程最大允许值
	int m_iSavingThreadPermittedNumber; // 允许的最大存储线程数， 默认为4

	// 系统参数
	int m_iChinaMarketRealtimeServer; // 中国市场实时数据服务器
	int m_iChinaMarketRTDataInquiryTime; // 中国市场实时数据查询间隔时间

	int m_iWorldMarketFinnhubInquiryTime; // 每次查询间隔时间，单位为毫秒
	int m_iWorldMarketTiingoInquiryTime; // 每次查询间隔时间，单位为毫秒
	int m_iWorldMarketQuandlInquiryTime; // 每次查询间隔时间，单位为毫秒

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
	int m_iSavingChinaMarketStockDayLineThread; // 存储日线历史数据时的并发线程数。
	bool m_bFastInquiringRTData;
	int m_iSinaRTDataInquiryPerTime; // 新浪实时数据每次查询股票数
	int m_iNeteaseRTDataInquiryPerTime; // 网易实时数据每次查询股票数
	int m_iTengxunRTDataInquiryPerTime; // 腾讯实时数据每次查询股票数

	// 网络数据更新频率（以天数记）
	int m_iStockProfileUpdateRate; // 默认45天更新一次
	int m_iInsideTransactionUpdateRate; // 默认45天更新一次
	int m_iInsideSentimentUpdateRate; // 默认45天更新一次
	int m_iStockBasicFinancialUpdateRate; // 默认45天更新一次
	int m_iStockPeerUpdateRate; // 默认90天更新一次

	// 测试系统
	CString m_strBenchmarkTestFileDirectory; // 性能测试文件所在的目录

	bool m_fUpdate;
	bool m_fInitialized = false;

	// 以下为无需存储之系统参数
	//
	// 具体工作计算机的参数
	RECT m_rSystemDisplay; // 显示器位素面积
	RECT m_rCurrentWindow; // 当前窗口位素面积

	// 必须将json变量放在最后。如果将nlohmann json变量放在前面，则导致赋值错误，目前原因不明（VS系统bug或者nholmann json库bug，两者必居其一）。
	// 是VS系统17.2.4的bug，17.2.5已经修正了这个bug。
	json m_systemConfiguration;
};

extern std::string gl_sSystemConfiguration; // 系统配置信息

extern CSystemConfiguration gl_systemConfiguration;
