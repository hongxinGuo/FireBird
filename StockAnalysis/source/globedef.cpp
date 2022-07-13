//////////////////////////////////////////////////////////////////////////////////////
//
// 为了保证有序的初始化，所有的全局变量皆应放在此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ThreadStatus.h"

#include"WebInquirer.h"

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

#include"WebRTDataContainer.h"
#include"HighPerformanceCounter.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"

#include "ChinaMarket.h"
#include"WorldMarket.h"

CSystemConfigeration gl_systemConfigeration; // 系统配置参数的总汇.此全局变量要位于所有全局变量的最前面，以保证第一个初始化。
CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在第二位，其他全局变量初始化时用到此变量（当报错时）。
CSystemStatus gl_systemStatus; // 系统状态。
CThreadStatus gl_ThreadStatus; // 系统中工作线程的各种状态，被各个工作线程所使用
CHighPerformanceCounter g_highPerformanceCounter;

CWebRTDataContainer gl_WebRTDataContainer; // 网络实时数据存储容器

// 为了事先初始化，信号量必须声明为全局变量
Semaphore gl_UpdateWorldMarketDB(1);  // 此信号量用于更新WorldMarket数据库
counting_semaphore<4> gl_UpdateWorldMarketDB2{ 4 };  // 此信号量用于更新WorldMarket数据库
Semaphore gl_SaveThreadPermitted(4);  // 此信号量用于生成日线历史数据库
Semaphore gl_BackGroundTaskThread(8); // 后台工作线程数。最大默认为8

CWebInquirer gl_WebInquirer;
CSystemData gl_SystemData;

// 网络数据提取器
CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // 新浪实时数据采集
CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // 腾讯实时数据采集
CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // 网易实时数据采集
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2 = nullptr; // 网易日线历史数据
CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry = nullptr; // Finnhub.io证券信息
CQuandlWebInquiryPtr gl_pQuandlWebInquiry = nullptr; // Quandl.com股票数据
CTiingoWebInquiryPtr gl_pTiingoWebInquiry = nullptr; // Tiingo.com股票数据

vector<CDataSourcePtr> gl_vDataSource;
CFinnhubDataSourcePtr gl_pDataSourceFinnhub = nullptr;
CTiingoDataSourcePtr gl_pDataSourceTiingo = nullptr;
CDataSourcePtr gl_pDataSourceQuandl = nullptr;

vector<CVirtualMarketPtr> gl_vMarketPtr{}; // 各市场指针的容器，只用于执行各市场的SchedulingTask

// 各市场。皆为唯一实例
CWorldMarketPtr gl_pWorldMarket = nullptr;
CChinaMarketPtr gl_pChinaMarket = nullptr; //此市场实例必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。//