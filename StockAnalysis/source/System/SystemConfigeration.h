//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 所有的全局参数皆位于此文件中，用于系统配置。
//
// 存储方式使用json格式，存储与文件SystemConfigeration.json中。
//
// 使用boost中的JSON库来解析之，实现序列化和反序列化。
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

using namespace std;
#include<string>
#include<map>

#include"nlohmannJsonDeclaration.h" // 按照顺序输出json，必须使用此ordered_json,以保证解析后的数据与解析前的顺序一致。

class CSystemConfigeration : CObject {
public:
	CSystemConfigeration();
	~CSystemConfigeration();

	bool LoadDB();
	bool SaveDB();
	void Update(void);
	void UpdateJson(void);

	void UpdateSystem(void); // 使用本参数去更新系统

	void SetDefaultFileName(CString fileName) { m_strFileName = fileName; }
	CString GetDefaultFileName(void) { return m_strFileName; }

	// 获取系统配置参数
	bool IsUsingFastCPU(void) noexcept { return m_bUsingFastCPU; }
	void SetUsingFastCPU(bool bUsingFastCPU) noexcept { m_bUsingFastCPU = bUsingFastCPU; }
	CString GetDatabaseAccountName(void) noexcept { return m_strDatabaseAccountName; }
	void SetDatabaseAccountName(CString strAccountName) noexcept { m_strDatabaseAccountName = strAccountName; }
	CString GetDatabaseAccountPassword(void) noexcept { return m_strDatabaseAccountPassword; }
	void SetDatabaseAccountPassword(CString strPassword) noexcept { m_strDatabaseAccountPassword = strPassword; }

	int GetBackgroundThreadPermittedNumber(void) noexcept { return m_iBackgroundThreadPermittedNumber; }
	int GetSavingThreadPermittedNumber(void) noexcept { return m_iSavingThreadPermittedNumber; }

	// 全局参数
	int GetChinaMarketRealtimeServer(void) noexcept { return m_iChinaMarketRealtimeServer; }
	void SetChinaMarketRealtimeServer(int iChinaMarketRealtimeServer) noexcept { m_fUpdate = true; m_iChinaMarketRealtimeServer = iChinaMarketRealtimeServer; }
	int GetChinaMarketRealtimeInquiryTime(void) noexcept { return m_iChinaMarketRealtimeInquiryTime; }
	void SetChinaMarketRealtimeInquiryTime(int iChinaMarketRealtimeInquiryTime) noexcept { m_fUpdate = true; m_iChinaMarketRealtimeInquiryTime = iChinaMarketRealtimeInquiryTime; }

	CString GetFinnhubToken(void) noexcept { return m_strFinnhubToken; }
	CString GetTiingoToken(void) noexcept { return m_strTiingoToken; }
	CString GetQuandlToken(void) noexcept { return m_strQuandlToken; }
	int GetWorldMarketFinnhubInquiryTime(void) noexcept { return m_iWorldMarketFinnhubInquiryTime; }
	void SetWorldMarketFinnhubInquiryTime(int iWorldMarketFinnhubInquiryTime) noexcept { m_fUpdate = true; m_iWorldMarketFinnhubInquiryTime = iWorldMarketFinnhubInquiryTime; }
	int GetWorldMarketTiingoInquiryTime(void) noexcept { return m_iWorldMarketTiingoInquiryTime; }
	void SetWorldMarketTiingoInquiryTime(int iWorldMarketTiingoInquiryTime) noexcept { m_fUpdate = true; m_iWorldMarketTiingoInquiryTime = iWorldMarketTiingoInquiryTime; }
	int GetWorldMarketQuandlInquiryTime(void) noexcept { return m_iWorldMarketQuandlInquiryTime; }
	void SetWorldMarketQuandlInquiryTime(int iWorldMarketQuandlInquiryTime) noexcept { m_fUpdate = true; m_iWorldMarketQuandlInquiryTime = iWorldMarketQuandlInquiryTime; }

	bool IsUsingSinaRTServer(void) const noexcept { if (m_iChinaMarketRealtimeServer == 0) return true; else return false; }
	bool IsUsingNeteaseRTServer(void) const noexcept { if (m_iChinaMarketRealtimeServer == 1) return true; else return false; }

	// WebSocket
	bool IsUsingFinnhubWebSocket(void) const noexcept { return m_bUsingFinnhubWebSocket; }
	void SetUsingFinnhubWebSocket(bool bUsingFinnhubWebSocket) noexcept { m_bUsingFinnhubWebSocket = bUsingFinnhubWebSocket; }
	bool IsUsingTiingoIEXWebSocket(void) const noexcept { return m_bUsingTiingoIEXWebSocket; }
	void SetUsingTiingoIEXWebSocket(bool bUsingTiingoIEXWebSocket) noexcept { m_fUpdate = true; m_bUsingTiingoIEXWebSocket = bUsingTiingoIEXWebSocket; }
	bool IsUsingTiingoCryptoWebSocket(void) const noexcept { return m_bUsingTiingoCryptoWebSocket; }
	void SetUsingTiingoCryptoWebSocket(bool bUsingTiingoCryptoWebSocket) noexcept { m_fUpdate = true; m_bUsingTiingoCryptoWebSocket = bUsingTiingoCryptoWebSocket; }
	bool IsUsingTiingoForexWebSocket(void) const noexcept { return m_bUsingTiingoForexWebSocket; }
	void SetUsingTiingoForexWebSocket(bool bUsingTiingoForexWebSocket) noexcept { m_fUpdate = true; m_bUsingTiingoForexWebSocket = bUsingTiingoForexWebSocket; }
	bool IsUsingQuandlWebSocket(void) const noexcept { return m_bUsingQuandlWebSocket; }
	void SetUsingQuandlWebSocket(bool bUsingQuandlWebSocket) noexcept { m_fUpdate = true; m_bUsingQuandlWebSocket = bUsingQuandlWebSocket; }

	// ChinaMarket
	int GetSavingChinaMarketStockDayLineThread(void) noexcept { return m_iSavingChinaMarketStockDayLineThread; }
	void SetSavingChinaMarketStockDayLineThread(int iSavingChinaMarketStockDayLineThread) noexcept { m_fUpdate = true; m_iSavingChinaMarketStockDayLineThread = iSavingChinaMarketStockDayLineThread; }
	bool IsFastInquiringRTData(void) noexcept { return m_bFastInquiringRTData; }
	void SetFastInquiringRTData(bool fFlag) noexcept { m_fUpdate = true; m_bFastInquiringRTData = fFlag; }

	// data update rate
	int GetStockProfileUpdateRate(void) noexcept { return m_iStockProfileUpdateRate; }
	void SetStockProfileUpdateRate(int iRate) noexcept { m_fUpdate = true; m_iStockProfileUpdateRate = iRate; }
	int GetStockBasicFinancialUpdateRate(void) noexcept { return m_iStockBasicFinancialUpdateRate; }
	void SetStockBasicFinancialUpdateRate(int iRate) noexcept { m_fUpdate = true; m_iStockBasicFinancialUpdateRate = iRate; }
	int GetInsideTransactionUpdateRate(void) noexcept { return m_iInsideTransactionUpdateRate; }
	void SetInsideTransactionUpdateRate(int iRate) noexcept { m_fUpdate = true; m_iInsideTransactionUpdateRate = iRate; }
	int GetInsideSentimentUpdateRate(void) noexcept { return m_iInsideSentimentUpdateRate; }
	void SetInsideSentimentUpdateRate(int iRate) noexcept { m_fUpdate = true; m_iInsideSentimentUpdateRate = iRate; }
	int GetStockPeerUpdateRate(void) noexcept { return m_iStockPeerUpdateRate; }
	void SetStockPeerUpdateRate(int iRate) noexcept { m_fUpdate = true; m_iStockPeerUpdateRate = iRate; }

	void SetSystemDisplayRect(RECT rect) noexcept { m_rSystemDisplay = rect; }
	void SetSystemDisplayRect(int cx, int cy) noexcept { m_rSystemDisplay.left = 0; m_rSystemDisplay.right = cx; m_rSystemDisplay.top = 0; m_rSystemDisplay.bottom = cy; }
	RECT GetSystemDisplayRect(void) noexcept { return m_rSystemDisplay; }
	int GetSystemDisplayHeight(void) noexcept { return m_rSystemDisplay.bottom - m_rSystemDisplay.top; }
	int GetSystemDisplayWidth(void) noexcept { return m_rSystemDisplay.right - m_rSystemDisplay.left; }
	void SetCurrentWindowRect(RECT rect) noexcept { m_rCurrentWindow = rect; }
	void SetCurrentWindowRect(int cx, int cy) noexcept { m_rCurrentWindow.left = 0; m_rCurrentWindow.right = cx; m_rCurrentWindow.top = 0; m_rCurrentWindow.bottom = cy; }
	RECT GetCurrentWindowRect(void) noexcept { return m_rCurrentWindow; }
	int GetCurrentWindowHeight(void) noexcept { return m_rCurrentWindow.bottom - m_rCurrentWindow.top; }
	int GetCurrentWindowWidth(void) noexcept { return m_rCurrentWindow.right - m_rCurrentWindow.left; }

	bool IsInitialized(void) const noexcept { return m_fInitialized; }

public:

protected:
	CString m_strFileName; // 配置文件名称

	// 系统配置
	bool m_bUsingFastCPU; //  是否使用快速CPU
	CString m_strDatabaseAccountName; // 数据库账户名称
	CString m_strDatabaseAccountPassword; // 数据库账户密码
	int m_iBackgroundThreadPermittedNumber; // 后台线程最大允许值
	int m_iSavingThreadPermittedNumber; // 允许的最大存储线程数， 默认为4

	// 系统参数
	int m_iChinaMarketRealtimeServer; // 中国市场实时数据服务器
	int m_iChinaMarketRealtimeInquiryTime; // 中国市场实时数据查询间隔时间

	int m_iWorldMarketFinnhubInquiryTime;
	int m_iWorldMarketTiingoInquiryTime;
	int m_iWorldMarketQuandlInquiryTime;

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

	// 网络数据更新频率（以天数记）
	int m_iStockProfileUpdateRate; // 默认45天更新一次
	int m_iInsideTransactionUpdateRate; // 默认45天更新一次
	int m_iInsideSentimentUpdateRate; // 默认45天更新一次
	int m_iStockBasicFinancialUpdateRate; // 默认45天更新一次
	int m_iStockPeerUpdateRate; // 默认90天更新一次

	bool m_fUpdate;
	bool m_fInitialized = false;

	// 以下为无需存储之系统参数
	//
	// 具体工作计算机的参数
	RECT m_rSystemDisplay; // 显示器位素面积
	RECT m_rCurrentWindow; // 当前窗口位素面积

	// 必须将json变量放在最后。如果将nlohmann json变量放在前面，则导致赋值错误，目前原因不明（VS系统bug或者nholmann json库bug，两者必居其一）。
	// 是VS系统17.2.4的bug，17.2.5已经修正了这个bug。
	json m_systemConfigeration;
};

extern	std::string gl_sSystemConfigeration; // 系统配置信息

extern CSystemConfigeration gl_systemConfigeration;
