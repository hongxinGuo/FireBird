//////////////////////////////////////////////////////////////////////////////////////
//
// Note 为了保证有序初始化，所有的全局变量皆应放在此文件中。
//
// 默认状态为正常工作状态，装入工作数据库；测试状态由测试主函数设定，重新装入测试数据库。
//
///////////////////////////////////////////////////////////////////////////////////////
module;
#include <concurrentqueue/moodycamel/concurrentqueue.h>

#include"concurrencpp/concurrencpp.h"
#include"spdlog/spdlog.h"
#include"stdafx.h"

module GlobeDef;

import SystemData;
import SystemMessage;
import MarketTask;

import WebRTData;
import DayLineWebData;

import FinnhubInquiryType;
import InaccessibleSymbol;

import FinnhubDataSource;
import SinaRTDataSource;
import TengxunRTDataSource;
import TengxunDayLineDataSource;
import TiingoDataSource;

import FinnhubWebSocket;
import TiingoCryptoWebSocket;
import TiingoForexWebSocket;
import TiingoIEXWebSocket;

import VirtualMarket;
import ChinaMarket;
import WorldMarket;

import AccessoryDataSource;

import ContainerChosenCrypto;

import AlphaVantageDataSource;
import ContainerChinaStock;
import ContainerChosenForex;
import ContainerFinnhubCountry;
import ContainerFinnhubCrypto;
import ContainerFinnhubCryptoExchange;
import ContainerFinnhubEconomicCalendar;
import ContainerFinnhubForexExchange;
import ContainerFinnhubForexSymbol;
import ContainerStockExchange;
import ContainerFinnhubStock;
import ContainerStockSymbol;
import ContainerTiingoChosenStock;
import ContainerTiingoCryptoSymbol;
import ContainerTiingoFundamentalDefinition;
import ContainerTiingoStock;
import ContainerTiingoSymbol;
import CountableSemaphore;
import EastmoneyDayLineDataSource;
import SystemConfiguration;

using namespace concurrencpp;

import std;
using std::shared_ptr;
using std::binary_semaphore;
using std::array;
using std::vector;
using std::string;
using std::literals::chrono_literals::operator ""ms;

HANDLE gl_hFireBirdMutex{ nullptr };

// 以下变量皆为唯一实例
CSystemConfiguration gl_systemConfiguration; // 系统配置参数的总汇.此全局变量要位于所有全局变量的最前面，以保证第一个初始化。
CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在第二位，其他全局变量初始化时用到此变量（当报错时）。
CSystemData gl_SystemData;

CFinnhubInquiryType gl_FinnhubInquiryType;
CInaccessibleSymbol gl_finnhubInaccessibleExchange{ "FinnhubInaccessibleExchange.json" }; // finnhub禁止访问交易所名单（免费账户无法访问的交易所数据）。唯一实例
CInaccessibleSymbol gl_tiingoInaccessibleStock{ "TiingoInaccessibleStock.json" }; // tiingo禁止访问股票名单（免费账户无法访问的股票数据）。唯一实例

std::chrono::sys_seconds gl_tpNow; // 当前系统时钟的时间戳， 所有的市场使用同一个协调世界时（Coordinated Universal Time）
const std::chrono::time_zone* gl_pTimeZoneLocal; // 软件运行所在的当地时区

// 为了事先初始化，信号量必须声明为全局变量
binary_semaphore gl_ProcessChinaMarketRTData{ 1 }; // 当处理中国市场的实时数据时，不允许同时存储之。
CCountableSemaphore gl_BackgroundWorkingThread; // 最多后台工作线程允许数量，使用系统配置设定为实际大小

concurrencpp::runtime gl_runtime; // 工作线程运行调度器
concurrencpp::thread_pool_executor gl_webInquiryExecutor{ "WebInquiry", 4, 100ms }; // 线程池工作线程运行调度器

long gl_concurrency_level = 4; // 并行计算允许最大数量。默认为四个协程。目前八核状态下，更多的更多的协程并不能提升效率。
array<timer, TASK_END> gl_aTimer; // timer序列。所有的任务如果使用单独timer的话，将对应的timer存储于此序列中。

int64_t gl_TiingoTotalData = 0;
int64_t gl_FinnhubTotalData = 0;
int64_t gl_ChinaMarketTotalData = 0;

vector<string> gl_vNasdaq100Stocks; // Nasdaq100指数股票代码

UINT gl_MsgFireBirdExit{ 0 };
UINT gl_MsgFireBirdSchedulingExit{ 0 };
UINT gl_MsgFireBirdCheckRunningExit{ 0 };
UINT gl_MsgFireBirdRunning{ 0 };

// Data source, 皆为唯一实例
CSinaRTDataSourcePtr gl_pSinaRTDataSource = nullptr;
CTengxunRTDataSourcePtr gl_pTengxunRTDataSource = nullptr;
CTengxunDayLineDataSourcePtr gl_pTengxunDayLineDataSource = nullptr;
CEastmoneyDayLineDataSourcePtr gl_pEastmoneyDayLineDataSource = nullptr;

CFinnhubDataSourcePtr gl_pFinnhubDataSource = nullptr;
CTiingoDataSourcePtr gl_pTiingoDataSource = nullptr;
CAlphaVantageDataSourcePtr gl_pAlphaVantageDataSource = nullptr;
CAccessoryDataSourcePtr gl_pAccessoryDataSource = nullptr;

// Web socket， 皆为唯一实例。
CFinnhubWebSocketPtr gl_pFinnhubWebSocket;
CTiingoIEXWebSocketPtr gl_pTiingoIEXWebSocket;
CTiingoForexWebSocketPtr gl_pTiingoForexWebSocket;
CTiingoCryptoWebSocketPtr gl_pTiingoCryptoWebSocket;

// 处理后的数据
moodycamel::ConcurrentQueue<CWebRTDataPtr> gl_qChinaMarketRTData(100000); // 中国市场新浪实时数据队列。
moodycamel::ConcurrentQueue<CDayLineWebDataPtr> gl_qDayLine(1000); // 日线数据

// ChinaMarket处理的数据
CContainerChinaStock gl_dataContainerChinaStock;
CContainerStockSymbol gl_dataContainerChinaStockSymbol;

// World market处理的数据
CContainerFinnhubForexSymbol gl_dataFinnhubForexSymbol;
CContainerFinnhubCrypto gl_dataFinnhubCryptoSymbol;
CContainerFinnhubCountry gl_dataContainerFinnhubCountry;
CContainerFinnhubEconomicCalendar gl_dataContainerFinnhubEconomicCalendar;

CContainerStockExchange gl_dataContainerStockExchange; // 直接装入预存交易所数据，
CContainerFinnhubForexExchange gl_dataContainerFinnhubForexExchange;
CContainerFinnhubCryptoExchange gl_dataContainerFinnhubCryptoExchange;

CContainerFinnhubStock gl_dataContainerFinnhubStock;
CContainerTiingoStock gl_dataContainerTiingoStock;
CContainerTiingoSymbol gl_dataContainerTiingoNewSymbol; // 存储每日查询后新增的股票指针
CContainerTiingoSymbol gl_dataContainerTiingoDelistedSymbol; // 存储已退市的股票指针
CContainerTiingoCryptoSymbol gl_dataContainerTiingoCryptoSymbol;
CContainerTiingoFundamentalDefinition gl_dataContainerTiingoFundamentalDefinition;

CContainerTiingoChosenStock gl_dataContainerTiingoChosenStock;
CContainerChosenForex gl_dataContainerChosenWorldForex;
CContainerChosenCrypto gl_dataContainerChosenWorldCrypto;

// 各市场。皆为唯一实例。为最后声明的变量。
CWorldMarketPtr gl_pWorldMarket = nullptr; //此实例必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。//
CChinaMarketPtr gl_pChinaMarket = nullptr; //此实例必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。//

vector<CVirtualMarketPtr> gl_vMarket{}; // 各市场指针的容器，只用于执行各市场的ScheduleTask
