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

#include "NeteaseRTDataSource.h"
#include "SinaRTDataSource.h"

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
	void SaveDB() const;
	void Update();

	void ClearJson();
	void UpdateJson();

	void UpdateSystem() const; // 使用本参数去更新系统

	void ChangeFinnhubAccountTypeToFree();
	void ChangeFinnhubAccountTypeToPaid();
	bool IsPaidTypeFinnhubAccount() const noexcept { return m_bFinnhubAccountFeePaid; }

	void SetConfigurationFileDirectory(const CString& fileDirectory) { m_strDirectory = fileDirectory; }
	[[nodiscard]] CString GetConfigurationFileDirectory() { return m_strDirectory; }
	void SetDefaultFileName(const CString& fileName) { m_strFileName = fileName; }
	[[nodiscard]] CString GetDefaultFileName() { return m_strFileName; }
	[[nodiscard]] CString GetConfigurationFileDirectoryAndName() const { return m_strDirectory + m_strFileName; }

	// 获取系统配置参数
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
	[[nodiscard]] int GetWorldMarketFinnhubInquiryTime() const noexcept { return m_iWorldMarketFinnhubInquiryTime; } // 单位为毫秒
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

	// 测试系统选项

	[[nodiscard]] bool IsInitialized() const noexcept { return m_fInitialized; }

	bool IsNeedUpdate() const noexcept { return m_fUpdate; }
	void SetUpdate(bool fFlag) noexcept { m_fUpdate = fFlag; }

	// 无需存储
	void SetExitingSystem(const bool bExit) { m_fExitingSystem = bExit; }
	bool IsExitingSystem() { return m_fExitingSystem; }
	void SetExitingCalculatingRS(const bool bExit) { m_fExitingCalculatingRS = bExit; }
	bool IsExitingCalculatingRS() const { return m_fExitingCalculatingRS; }
	void SetWorkingMode(const bool bNormal) { m_fWorkingMode = bNormal; }
	bool IsWorkingMode() const { return m_fWorkingMode; }

	// 当下载新浪或者网易实时数据出现问题时，系统的其他网络活动应该让步。
	static bool IsWebBusy() { return gl_pSinaRTDataSource->IsWebError() || gl_pNeteaseRTDataSource->IsWebError(); }

	bool IsInitialized() { return sm_bInitialized; }

protected:
	static bool sm_bInitialized; // 使用静态变量来保证只生成唯一实列。
	CString m_strDirectory; // 配置文件目录
	CString m_strFileName; // 配置文件名称

	// 系统配置
	bool m_bDebugMode; // 系统是否是测试状态
	bool m_bReloadSystem; // 系统是否允许周期性重启
	CString m_strDatabaseAccountName; // 数据库账户名称
	CString m_strDatabaseAccountPassword; // 数据库账户密码
	int m_iBackgroundThreadPermittedNumber; // 后台线程最大允许值

	// 系统参数
	int m_iChinaMarketRealtimeServer; // 中国市场实时数据服务器.0:新浪实时数据服务器； 1:网易实时数据服务器。
	int m_iChinaMarketDayLineServer; // 中国市场日线数据服务器。0:网易日线服务器；1:腾讯日线服务器。
	int m_iChinaMarketRTDataInquiryTime; // 中国市场实时数据查询间隔时间

	// World Market
	CString m_strFinnhubToken; // 令牌
	CString m_strTiingoToken; // 令牌
	CString m_strQuandlToken; // 令牌
	bool m_bFinnhubAccountFeePaid; // 付费账户或者免费账户
	bool m_bTiingoAccountFeePaid;
	bool m_bQuandlAccountFeePaid;
	int m_iWorldMarketFinnhubInquiryTime; // 每次查询间隔时间，单位为毫秒。付费账户每分钟300次（实时数据为900次），免费账户每分钟60次。
	int m_iWorldMarketTiingoInquiryTime; // 每次查询间隔时间，单位为毫秒。付费账户每小时5000次，免费账户每小时50次。
	int m_iWorldMarketQuandlInquiryTime; // 每次查询间隔时间，单位为毫秒

	// WebSocket
	bool m_bUsingFinnhubWebSocket;
	bool m_bUsingTiingoIEXWebSocket;
	bool m_bUsingTiingoCryptoWebSocket;
	bool m_bUsingTiingoForexWebSocket;
	bool m_bUsingQuandlWebSocket;

	// ChinaMarket
	int m_iSavingChinaMarketStockDayLineThread; // 存储日线历史数据时的并发线程数。
	bool m_bFastInquiringRTData;
	int m_iNumberOfRTDataSource; // 实时数据申请引擎数， 默认为4
	int m_iSinaRTDataInquiryPerTime; // 新浪实时数据每次查询股票数
	int m_iNeteaseRTDataInquiryPerTime; // 网易实时数据每次查询股票数
	int m_iTengxunRTDataInquiryPerTime; // 腾讯实时数据每次查询股票数

	// 网络数据更新频率（以天数记）
	int m_iStockProfileUpdateRate; // 默认45天更新一次
	int m_iInsideTransactionUpdateRate; // 默认45天更新一次
	int m_iInsideSentimentUpdateRate; // 默认45天更新一次
	int m_iStockBasicFinancialUpdateRate; // 默认45天更新一次
	int m_iStockPeerUpdateRate; // 默认90天更新一次
	int m_iEPSSurpriseUpdateRate; // 默认90天更新一次

	// 测试系统
	CString m_strBenchmarkTestFileDirectory; // 性能测试文件所在的目录

	bool m_fUpdate;
	bool m_fInitialized = false;

	// 以下为无需存储之系统参数
	//
	// 具体工作计算机的参数
	RECT m_rSystemDisplay; // 显示器位素面积
	RECT m_rCurrentWindow; // 当前窗口位素面积

	json m_systemConfiguration;

	//无需存储的变量
	std::atomic_bool m_fExitingSystem{false}; //  系统退出标识，用于终止其他线程。
	bool m_fExitingCalculatingRS{false}; // 用于通知工作线程退出的信号
	bool m_fWorkingMode{false}; // 正常模式标识，默认为假。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。
};

extern std::string gl_sSystemConfiguration; // 系统配置信息

extern CSystemConfiguration gl_systemConfiguration;
