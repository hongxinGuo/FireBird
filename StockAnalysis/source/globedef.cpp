//////////////////////////////////////////////////////////////////////////////////////
//
// 所有的全局变量皆应放在此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"
#include"SemaphoreDef.h"

#include "ChinaMarket.h"
#include"WorldMarket.h"

#include"SystemMessage.h"
#include"SystemData.h"
#include"WebInquirer.h"

#include"SinaRTWebInquiry.h"
#include"TengxunRTWebInquiry.h"
#include"NeteaseRTWebInquiry.h"
#include"NeteaseDayLineWebInquiry.h"
#include"FinnhubWebInquiry.h"
#include"TiingoWebInquiry.h"
#include"QuandlWebInquiry.h"

CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在第一位，其他全局变量初始化时用到此变量。

CThreadStatus gl_ThreadStatus; // 系统中的各种状态，被各个工作线程所使用

deque<CString> gl_dequeRTStockInquire; // 申请实时股票信息的股票队列（尚未使用）
deque<CString> gl_dequeDayLineStockInquire; // 申请日线历史数据的股票队列（尚未使用）

atomic_bool gl_fExitingSystem = false; //  系统退出标识，用于终止其他线程。
bool gl_fExitingCalculatingRS = false; // 用于通知工作线程退出的信号

bool gl_fNormalMode = false; // 测试模式标识，默认为真。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。

CWebRTDataContainer gl_WebRTDataContainer; // 网络实时数据存储容器

// 信号量必须声明为全局变量（为了初始化）
Semaphore gl_UpdateWorldMarketDB(1);  // 此信号量用于生成美国股票日线历史数据库
Semaphore gl_SaveOneStockDayLine(4);  // 此信号量用于生成日线历史数据库
Semaphore gl_SemaphoreBackGroundTaskThreads(cMaxBackGroundTaskThreads); // 后台工作线程数。最大默认为8

CWebInquirer gl_WebInquirer;
CSystemData gl_SystemData;

int gl_iMaxSavingOneDayLineThreads = 4; // 允许存储日线数据的线程数。此变量取值范围为1-4
// 网络数据提取器
CSinaRTWebInquiryPtr gl_pSinaRTWebInquiry = nullptr; // 新浪实时数据采集
CTengxunRTWebInquiryPtr gl_pTengxunRTWebInquiry = nullptr; // 腾讯实时数据采集
CNeteaseRTWebInquiryPtr gl_pNeteaseRTWebInquiry = nullptr; // 网易实时数据采集
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry = nullptr; // 网易日线历史数据
CNeteaseDayLineWebInquiryPtr gl_pNeteaseDayLineWebInquiry2 = nullptr; // 网易日线历史数据
CFinnhubWebInquiryPtr gl_pFinnhubWebInquiry = nullptr; // Finnhub.io证券信息
CQuandlWebInquiryPtr gl_pQuandlWebInquiry = nullptr;
CTiingoWebInquiryPtr gl_pTiingoWebInquiry = nullptr;

vector<CVirtualMarketPtr> gl_vMarketPtr{}; // 各市场指针的容器，只用于执行各市场的SchedulingTask

// 各市场。皆为唯一实例
CWorldMarketPtr gl_pWorldMarket = nullptr;
CChinaMarketPtr gl_pChinaMarket = nullptr; //此市场实例必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。//