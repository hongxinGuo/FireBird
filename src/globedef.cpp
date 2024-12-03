//////////////////////////////////////////////////////////////////////////////////////
//
// 为了保证有序的初始化，所有的全局变量皆应放在此文件中。
//
// 默认状态为正常工作状态，装入工作数据库；测试状态由测试主函数设定，重新装入测试数据库。
//
///////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"FinnhubInquiryType.h"
#include"FinnhubInaccessibleExchange.h"
#include "TiingoInaccessibleStock.h"

#include"FinnhubDataSource.h"
#include"NeteaseDayLineDataSource.h"
#include"NeteaseRTDataSource.h"
#include"QuandlDataSource.h"
#include"SinaRTDataSource.h"
#include"TengxunRTDataSource.h"
#include"TengxunDayLineDataSource.h"
#include"TiingoDataSource.h"

#include"FinnhubWebSocket.h"
#include"TiingoCryptoWebSocket.h"
#include"TiingoForexWebSocket.h"
#include"TiingoIEXWebSocket.h"

#include "ChinaMarket.h"
#include "WorldMarket.h"

#include"thread.h"
#include"threadStatus.h"

#undef max
#include"concurrencpp/concurrencpp.h"
using namespace concurrencpp;

HANDLE gl_hFireBirdMutex{ nullptr };

shared_ptr<spdlog::logger> gl_dailyLogger = nullptr;
shared_ptr<spdlog::logger> gl_traceLogger; // 跟踪日志，用于系统调试
shared_ptr<spdlog::logger> gl_warnLogger; // 警告跟踪日志，用于系统调试
shared_ptr<spdlog::logger> gl_dailyWebSocketLogger = nullptr;
shared_ptr<spdlog::logger> gl_SoftwareDevelopingLogger = nullptr;

// 以下变量皆为唯一实例
CSystemConfiguration gl_systemConfiguration; // 系统配置参数的总汇.此全局变量要位于所有全局变量的最前面，以保证第一个初始化。
CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在第二位，其他全局变量初始化时用到此变量（当报错时）。
CThreadStatus gl_ThreadStatus; // 系统中工作线程的各种状态，被各个工作线程所使用
CSystemData gl_SystemData;

CFinnhubInquiryType gl_FinnhubInquiryType;
CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange; // finnhub禁止访问交易所名单（免费账户无法访问的交易所数据）。唯一实例
CTiingoInaccessibleStock gl_tiingoInaccessibleStock;

std::chrono::sys_seconds gl_tpNow; // 当前系统时钟的时间戳， 所有的市场使用同一个协调世界时（Coordinated Universal Time）

// 为了事先初始化，信号量必须声明为全局变量
binary_semaphore gl_UpdateChinaMarketDB{ 1 }; // 用于更新ChinaMarket数据库。todo 由于对MySQL数据库不太了解，偶尔会出现存储问题，我估计与同步有关，故而设置互斥变量
binary_semaphore gl_UpdateWorldMarketDB{ 1 }; // 用于更新WorldMarket数据库。todo 由于对MySQL数据库不太了解，偶尔会出现存储问题，我估计与同步有关，故而设置互斥变量
binary_semaphore gl_ProcessChinaMarketRTData{ 1 }; // 当处理中国市场的实时数据时，不允许同时存储之。
counting_semaphore<__MAX_BACKGROUND_WORKING_THREAD__> gl_BackgroundWorkingThread{ __MAX_BACKGROUND_WORKING_THREAD__ }; // 最多后台工作线程允许数量，使用系统配置设定为实际大小

concurrencpp::runtime gl_runtime; // 工作线程运行调度器
concurrencpp::runtime gl_backgroundRuntime; // 后台工作线程运行调度器
int gl_concurrency_level = 4; // 并行计算允许最大数量。默认为四个协程
array<timer, TASK_END> gl_aTimer; // timer序列。所有的任务如果使用单独timer的话，将对应的timer存储于此序列中。

// Data source, 皆为唯一实例
CSinaRTDataSourcePtr gl_pSinaRTDataSource = nullptr;
CTengxunRTDataSourcePtr gl_pTengxunRTDataSource = nullptr;
CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource = nullptr;
CNeteaseDayLineDataSourcePtr gl_pNeteaseDayLineDataSource = nullptr;
CTengxunDayLineDataSourcePtr gl_pTengxunDayLineDataSource = nullptr;
CFinnhubDataSourcePtr gl_pFinnhubDataSource = nullptr;
CTiingoDataSourcePtr gl_pTiingoDataSource = nullptr;
CQuandlDataSourcePtr gl_pQuandlDataSource = nullptr;

// Web socket， 皆为唯一实例。
CFinnhubWebSocketPtr gl_pFinnhubWebSocket;
CTiingoIEXWebSocketPtr gl_pTiingoIEXWebSocket;
CTiingoForexWebSocketPtr gl_pTiingoForexWebSocket;
CTiingoCryptoWebSocketPtr gl_pTiingoCryptoWebSocket;

// 处理后的数据
ConcurrentQueue<CWebRTDataPtr> gl_qChinaMarketRTData(10000); // 中国市场新浪实时数据队列。
ConcurrentQueue<CDayLineWebDataPtr> gl_qDayLine(1000); // 日线数据

// ChinaMarket处理的数据
CContainerChinaStock gl_dataContainerChinaStock;
CContainerStockSymbol gl_dataContainerChinaStockSymbol;

// World market处理的数据
CContainerFinnhubForexSymbol gl_dataFinnhubForexSymbol;
CContainerFinnhubCrypto gl_dataFinnhubCryptoSymbol;
CContainerFinnhubCountry gl_dataContainerFinnhubCountry;
CContainerFinnhubEconomicCalendar gl_dataContainerFinnhubEconomicCalendar;

CContainerFinnhubStockExchange gl_dataContainerFinnhubStockExchange;
CContainerFinnhubForexExchange gl_dataContainerFinnhubForexExchange;
CContainerFinnhubCryptoExchange gl_dataContainerFinnhubCryptoExchange;

CContainerFinnhubStock gl_dataContainerFinnhubStock;
CContainerTiingoStock gl_dataContainerTiingoStock;
CContainerTiingoSymbol gl_dataContainerTiingoNewSymbol; // 存储每日查询后新增的股票指针
CContainerTiingoSymbol gl_dataContainerTiingoDelistedSymbol; // 存储已退市的股票指针
CContainerTiingoCryptoSymbol gl_dataContainerTiingoCryptoSymbol;
CContainerTiingoFundamentalDefinition gl_dataContainerTiingoFundamentalDefinition;

CContainerChosenStock gl_dataContainerChosenFinnhubStock;
CContainerChosenForex gl_dataContainerChosenWorldForex;
CContainerChosenCrypto gl_dataContainerChosenWorldCrypto;

// 各市场。皆为唯一实例。为最后声明的变量。
CWorldMarketPtr gl_pWorldMarket = nullptr; //此实例必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。//
CChinaMarketPtr gl_pChinaMarket = nullptr; //此实例必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。//

vector<CVirtualMarketPtr> gl_vMarket{}; // 各市场指针的容器，只用于执行各市场的ScheduleTask
