//////////////////////////////////////////////////////////////////////////////////////
//
// 为了保证有序的初始化，所有的全局变量皆应放在此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include<ixwebsocket/IXWebSocket.h>

#include"ThreadStatus.h"

#include"HighPerformanceCounter.h"

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

#include"SinaRTDataSource.h"
#include"TengxunRTDataSource.h"
#include"NeteaseRTDataSource.h"
#include"NeteaseDaylineDataSource.h"
#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"
#include"QuandlDataSource.h"

#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoForexWebSocket.h"
#include"TiingoCryptoWebSocket.h"

#include "ChinaMarket.h"
#include"WorldMarket.h"

#include"FinnhubInaccessibleExchange.h"

CSystemConfigeration gl_systemConfigeration; // 系统配置参数的总汇.此全局变量要位于所有全局变量的最前面，以保证第一个初始化。
CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在第二位，其他全局变量初始化时用到此变量（当报错时）。
CSystemStatus gl_systemStatus; // 系统状态。
CThreadStatus gl_ThreadStatus; // 系统中工作线程的各种状态，被各个工作线程所使用

CFinnhubInaccessibleExchange gl_finnhubInaccessibleExchange; // finnhub禁止访问交易所（免费账户无法访问的交易所数据）

// 为了事先初始化，信号量必须声明为全局变量
binary_semaphore gl_UpdateWorldMarketDB{ 1 };  // 此信号量用于更新WorldMarket数据库
counting_semaphore<4> gl_SaveDayLineThreadPermitted{ 1 }; // 当数据库中没有日线数据时，增加此信号量最大值至4
counting_semaphore<8> gl_BackGroundTaskThread{ 8 };// 后台工作线程数。最大为8
binary_semaphore gl_ProcessChinaMarketRTData{ 1 }; // 处理中国市场的实时数据时，不允许同时存储之。

CSystemData gl_SystemData;

// 网络数据提取器
CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // 新浪实时数据采集
CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // 腾讯实时数据采集
CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // 网易实时数据采集
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // 网易日线历史数据
CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry = nullptr; // Finnhub.io证券信息
CQuandlWebInquiryPtr gl_pQuandlWebInquiry = nullptr; // Quandl.com股票数据
CTiingoWebInquiryPtr gl_pTiingoWebInquiry = nullptr; // Tiingo.com股票数据

// Data source
CSinaRTDataSourcePtr gl_pSinaRTDataSource = nullptr;
CTengxunRTDataSourcePtr gl_pTengxunRTDataSource = nullptr;
CNeteaseRTDataSourcePtr gl_pNeteaseRTDataSource = nullptr;
CNeteaseDaylineDataSourcePtr gl_pNeteaseDaylineDataSource = nullptr;
CFinnhubDataSourcePtr gl_pFinnhubDataSource = nullptr;
CTiingoDataSourcePtr gl_pTiingoDataSource = nullptr;
CQuandlDataSourcePtr gl_pQuandlDataSource = nullptr;

// Web socket
CFinnhubWebSocket gl_finnhubWebSocket;
CTiingoIEXWebSocket gl_tiingoIEXWebSocket;
CTiingoForexWebSocket gl_tiingoForexWebSocket;
CTiingoCryptoWebSocket gl_tiingoCryptoWebSocket;

vector<CVirtualMarketPtr> gl_vMarketPtr{}; // 各市场指针的容器，只用于执行各市场的SchedulingTask

// 各市场。皆为唯一实例。为最后声明的变量。
CWorldMarketPtr gl_pWorldMarket = nullptr;
CChinaMarketPtr gl_pChinaMarket = nullptr; //此实例必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。//