//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 所有的全局参数皆位于此文件中，用于系统配置。
//
// 存储方式使用json格式，存储于文件SystemConfiguration.json中。
//
// 使用Nlohmann的JSON库来解析之，实现序列化和反序列化。
//
// todo 准备使用simdjson来装载json数据，Nlohmannjson只用于存储json数据。
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"nlohmannJsonDeclaration.h" // 按照顺序输出json，必须使用此ordered_json,以保证解析后的数据与解析前的顺序一致。
#include"nlohmann/json.hpp"

class CSystemConfiguration final {
public:
	CSystemConfiguration();
	// 只能有一个实例,不允许赋值。
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

	// 环境配置
	int GetDisplayPropertyPage() const noexcept { return m_iDisplayPropertyPage; }
	void SetDisplayPropertyPage(int page) noexcept { m_iDisplayPropertyPage = page; }

	// 获取系统配置参数
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

	// 全局参数
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
	[[nodiscard]] int GetWorldMarketFinnhubInquiryTime() const noexcept { return m_iWorldMarketFinnhubInquiryTime; } // 单位为毫秒
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

	// 测试系统选项

	[[nodiscard]] bool IsInitialized() const noexcept { return m_fInitialized; }

	bool IsUpdateDB() const noexcept { return m_fUpdateDB; }
	void SetUpdateDB(bool fFlag) noexcept { m_fUpdateDB = fFlag; }

	// 无需存储
	void SetExitingSystem(const bool bExit) { m_fExitingSystem = bExit; }
	bool IsExitingSystem() { return m_fExitingSystem; }
	void SetExitingCalculatingRS(const bool bExit) { m_fExitingCalculatingRS = bExit; }
	bool IsExitingCalculatingRS() const { return m_fExitingCalculatingRS; }
	void SetWorkingMode(const bool bNormal) { m_fWorkingMode = bNormal; }
	bool IsWorkingMode() const { return m_fWorkingMode; }

	// 当下载新浪或者网易实时数据出现问题时，系统的其他网络活动应该让步。
	bool IsWebBusy();

	bool IsInitialized() { return sm_bInitialized; }

protected:
	static bool sm_bInitialized; // 使用静态变量来保证只生成唯一实列。
	CString m_strDirectory; // 配置文件目录
	CString m_strFileName; // 配置文件名称

	// 环境配置
	int m_iDisplayPropertyPage{ 0 }; // application : 0, 

	// 系统配置
	int m_iLogLevel; // spdlog日志文件记录等级: trace, debug, info, warn, error, critical, off
	bool m_bDebugMode{ false }; // 系统是否是测试状态
	bool m_bReloadSystem{ false }; // 系统是否允许周期性重启
	CString m_strDatabaseAccountName; // 数据库账户名称
	CString m_strDatabaseAccountPassword; // 数据库账户密码
	int m_iBackgroundThreadPermittedNumber; // 后台线程最大允许值

	// 系统参数
	int m_iChinaMarketRealtimeServer{ 0 }; // 中国市场实时数据服务器.0:新浪实时数据服务器； 1:网易实时数据服务器。
	int m_iChinaMarketDayLineServer{ 0 }; // 中国市场日线数据服务器。0:网易日线服务器；1:腾讯日线服务器。
	int m_iChinaMarketRTDataInquiryTime{ 250 }; // 中国市场实时数据查询间隔时间,单位为毫秒

	// World Market
	long m_lMarketResettingTime{ 170000 };
	CString m_strFinnhubToken; // 令牌
	CString m_strTiingoToken; // 令牌
	CString m_strQuandlToken; // 令牌
	bool m_bFinnhubAccountFeePaid{ true }; // 付费账户或者免费账户
	bool m_bTiingoAccountFeePaid{ true };
	bool m_bQuandlAccountFeePaid{ true };
	int m_iWorldMarketFinnhubInquiryTime{ 1100 }; // 每次查询间隔时间，单位为毫秒。付费账户每分钟300次（实时数据为900次），免费账户每分钟60次。
	int m_iWorldMarketTiingoInquiryTime{ 3600000 / 500 }; // 每次查询间隔时间，单位为毫秒。付费账户每小时5000次，免费账户每小时50次。
	int m_iWorldMarketQuandlInquiryTime{ 3600000 / 100 }; // 每次查询间隔时间，单位为毫秒
	CString m_strCurrentStock{ _T("") }; // 当前所选股票

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
	int m_iSavingChinaMarketStockDayLineThread{ 4 }; // 存储日线历史数据时的并发线程数。
	bool m_bFastInquiringRTData{ false };
	int m_iNumberOfRTDataSource{ 4 }; // 实时数据申请引擎数， 默认为4
	int m_iSinaRTDataInquiryPerTime{ 850 }; // 新浪实时数据每次查询股票数
	int m_iNeteaseRTDataInquiryPerTime{ 900 }; // 网易实时数据每次查询股票数
	int m_iTengxunRTDataInquiryPerTime{ 900 }; // 腾讯实时数据每次查询股票数

	// 网络数据更新频率（以天数记）
	// Finnhub数据
	int m_iStockProfileUpdateRate{ 365 }; // 默认365天更新一次
	int m_iInsideTransactionUpdateRate{ 30 }; // 默认30天更新一次
	int m_iInsideSentimentUpdateRate{ 30 }; // 默认30天更新一次
	int m_iStockBasicFinancialUpdateRate{ 45 }; // 默认45天更新一次
	int m_iStockPeerUpdateRate{ 90 }; // 默认90天更新一次
	int m_iEPSSurpriseUpdateRate{ 90 }; // 默认90天更新一次
	int m_iSECFilingsUpdateRate{ 30 }; // 默认30天更新一次
	// Tiingo 数据
	int m_iTiingoCompanyFinancialStateUpdateRate{ 45 }; // Tiingo公司金融数据更新

	// 测试系统
	CString m_strBenchmarkTestFileDirectory; // 性能测试文件所在的目录

	bool m_fUpdateDB{ false };
	bool m_fInitialized{ false };

	// 以下为无需存储之系统参数
	//
	// 具体工作计算机的参数
	RECT m_rSystemDisplay; // 显示器位素面积
	RECT m_rCurrentWindow; // 当前窗口位素面积

	//无需存储的变量
	std::atomic_bool m_fExitingSystem{ false }; //  系统退出标识，用于终止其他线程。
	bool m_fExitingCalculatingRS{ false }; // 用于通知工作线程退出的信号
	bool m_fWorkingMode{ true }; // 正常模式标识，默认为假。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。
};

extern std::string gl_sSystemConfiguration; // 系统配置信息

extern CSystemConfiguration gl_systemConfiguration;
