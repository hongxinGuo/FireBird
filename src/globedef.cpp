//////////////////////////////////////////////////////////////////////////////////////
//
// 为了保证有序的初始化，所有的全局变量皆应放在此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"FinnhubInquiryType.h"
#include"FinnhubInaccessibleExchange.h"

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

#include"ThreadStatus.h"

// 以下变量皆为唯一实例
CSystemConfiguration gl_systemConfiguration; // 系统配置参数的总汇.此全局变量要位于所有全局变量的最前面，以保证第一个初始化。
CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在第二位，其他全局变量初始化时用到此变量（当报错时）。
CThreadStatus gl_ThreadStatus; // 系统中工作线程的各种状态，被各个工作线程所使用

CFinnhubInquiryType gl_FinnhubInquiryType;

time_t gl_tUTCTime = 0; // 所有的市场使用同一个协调世界时（Coordinated Universal Time）

CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange; // finnhub禁止访问交易所名单（免费账户无法访问的交易所数据）。唯一实例

// 为了事先初始化，信号量必须声明为全局变量
binary_semaphore gl_UpdateChinaMarketDB{1}; // 用于更新ChinaMarket数据库。todo 由于我对MySQL数据库不太了解，偶尔会出现存储问题，我估计与同步有关，故而设置互斥变量
binary_semaphore gl_UpdateWorldMarketDB{1}; // 用于更新WorldMarket数据库。todo 由于我对MySQL数据库不太了解，偶尔会出现存储问题，我估计与同步有关，故而设置互斥变量
binary_semaphore gl_ProcessChinaMarketRTData{1}; // 当处理中国市场的实时数据时，不允许同时存储之。
counting_semaphore<3> gl_WebSourceParseAndStoreData{3};//用于解析WebSource中的数据。将ParseAndStoreData线程限制至最多3个，这样既能保证足够的计算速度，也不会发生系统颠簸。当改为4个时，就能观察到系统颠簸。
counting_semaphore<8> gl_BackgroundWorkingThread{8}; // 最多后台工作线程允许数量

CSystemData gl_SystemData;

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
CPriorityQueueWebRTData gl_qSinaRT; // 中国市场新浪实时数据队列。
CPriorityQueueWebRTData gl_qNeteaseRT; // 中国市场网易实时数据队列。
CPriorityQueueWebRTData gl_qTengxunRT; // 中国市场腾讯实时数据队列。
CTemplateMutexAccessQueue<CDayLineWebData> gl_qDayLine; // 日线数据

CContainerFinnhubForexSymbol gl_dataFinnhubForexSymbol;
CContainerFinnhubCryptoSymbol gl_dataFinnhubCryptoSymbol;
CContainerFinnhubCountry gl_dataFinnhubCountry;
CContainerFinnhubEconomicCalendar gl_dataFinnhubEconomicCalendar;

CContainerTiingoStock gl_dataTiingoStock;
CContainerTiingoCryptoSymbol gl_dataTiingoCryptoSymbol;

CContainerChosenStock gl_containerChosenStock;
CContainerChosenForex gl_containerChosenForex;
CContainerChosenCrypto gl_containerChosenCrypto;

// 各市场。皆为唯一实例。为最后声明的变量。
CWorldMarketPtr gl_pWorldMarket = nullptr;
CChinaMarketPtr gl_pChinaMarket = nullptr; //此实例必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。//

vector<CVirtualMarketPtr> gl_vMarketPtr{}; // 各市场指针的容器，只用于执行各市场的SchedulingTask
