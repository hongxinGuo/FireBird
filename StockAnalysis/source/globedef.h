#ifndef __GLOBE_DEFINE__
#define __GLOBE_DEFINE__

#include"stdafx.h"

enum {
  __STOCK_NOT_CHECKED__ = 128, // 尚未检查过的股票代码
  __STOCK_IPOED__ = 255, // 正常股票标识
  __STOCK_NULL__ = 0, // 无效代码（此代码的股票不存在）
  __STOCK_DELISTED__ = 1, // 已退市的股票标识
};

enum {
  __SHANGHAI_MARKET__ = 1, // 上海市场
  __SHENZHEN_MARKET__ = 2, // 深圳市场
  __SHANGHAI_INDEX__ = 3, // 上海指数
  __SHENZHEN_INDEX__ = 4, // 深圳指数
  __SHANGHAI_MAIN__ = 5, // 上海主板
  __SHENZHEN_MAIN__ = 6, // 深圳主板
  __SHANGHAI_3BAN__ = 7, // 上海3板
  __SHENZHEN_3BAN__ = 8, //深圳中小板
  __SHANGHAI_B_SHARE__ = 9, //上海B股
  __SHENZHEN_B_SHARE__ = 10, // 深圳B股
  __SHANGHAI_KECHUANG__ = 11, // 上海科创板
  __SHENZHEN_CHUANGYE__ = 12, // 深圳创业板
};

#include"RTData.h"

#include"SetDayLine.h"

#include"SinaWebRTData.h"
#include"TengxunWebRTData.h"
#include"NeteaseWebDayLineData.h"
#include"NeteaseWebRTData.h"
#include"CrweberIndexWebData.h"

#include"CrweberIndex.h"

#include"SystemMessage.h"
#include"QueueRTData.h"
#include"QueueWebRTData.h"
#include"PriorityQueueRTData.h"
#include"ThreadStatus.h"
#include"SystemTime.h"

#include"Semaphore.h"

using namespace std;
#include<deque>
#include<vector>
#include<map>
#include<deque>
#include<memory>
#include<atomic>

using namespace MyAccessory;

// 测试用变量
extern CRTDataPtr gl_TESTpRTData;

extern CSinaWebRTData gl_SinaWebRTData; // 新浪实时数据采集
extern CTengxunWebRTData gl_TengxunWebRTData; // 腾讯实时数据采集
extern CNeteaseWebRTData gl_NeteaseWebRTData; // 网易实时数据采集
extern CNeteaseWebDayLineData gl_NeteaseWebDayLineData; // 网易日线历史数据
extern CNeteaseWebDayLineData gl_NeteaseWebDayLineDataSecond; // 网易日线历史数据
extern CNeteaseWebDayLineData gl_NeteaseWebDayLineDataThird; // 网易日线历史数据
extern CNeteaseWebDayLineData gl_NeteaseWebDayLineDataFourth; // 网易日线历史数据
extern CNeteaseWebDayLineData gl_NeteaseWebDayLineDataFive; // 网易日线历史数据
extern CNeteaseWebDayLineData gl_NeteaseWebDayLineDataSix; // 网易日线历史数据
extern CCrweberIndexWebData gl_CrweberIndexWebData;   // crweber.com上的每日油运指数

extern CCrweberIndex gl_CrweberIndex;
extern CCrweberIndex gl_CrweberIndexLast;

extern CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在第一位，其他全局变量初始化时用到此变量。
extern CSystemTime gl_systemTime; // 系统时间汇总。

extern CPriorityQueueRTData gl_QueueSinaRTData; // 系统实时数据队列。
extern CQueueRTData gl_QueueSinaRTDataForSave; // 用于存储的新浪实时数据队列
extern CPriorityQueueRTData gl_QueueTengxunRTData; // 系统实时数据队列。
extern CPriorityQueueRTData gl_QueueNeteaseRTData; // 系统实时数据队列。

extern CQueueWebRTData gl_QueueSinaWebRTData; // 新浪网络数据暂存队列
extern CQueueWebRTData gl_QueueTengxunWebRTData; // 腾讯网络数据暂存队列
extern CQueueWebRTData gl_QueueNeteaseWebRTData; // 网易网络数据暂存队列
extern CQueueWebRTData gl_QueueCrweberdotcomWebData; // crweber.com网络数据暂存队列

extern CThreadStatus gl_ThreadStatus; // 系统中的各种状态，被各个工作线程所使用

extern deque<CString> gl_dequeRTStockInquire; // 申请实时股票信息的股票队列
extern deque<CString> gl_dequeDayLineStockInquire; // 申请日线历史数据的股票队列。

extern atomic<bool> gl_ExitingSystem; //  系统退出标识，用于终止其他线程。
extern bool gl_fExitingCalculatingRS; // 用于通知工作线程退出的信号
extern bool gl_fResetSystem; // 系统重启标识

extern bool gl_fNormalMode; // 正常模式标识，默认为假。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。
extern bool gl_fTestMode; // 是否设置了gl_fTestMode标识（用于判断是否在实际系统中使用了MySQLTest驱动）。

extern const int gl_cMaxSavingOneDayLineThreads;
// 信号量必须声明为全局变量（为了初始化）
extern Semaphore gl_SaveOneStockDayLine;  // 此信号量用于生成日线历史数据库
extern Semaphore gl_SemaphoreCalculateDayLineRS;
extern Semaphore gl_ProcessSinaRTDataQueue;
extern Semaphore gl_ProcessTengxunRTDataQueue;
extern Semaphore gl_ProcessNeteaseRTDataQueue;

class CMarket;
// 此市场变量必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。
extern CMarket gl_ChinaStockMarket; // 市场。所有活跃的股票皆位于其中，单一实例变量，仅允许存在一个实例。

#endif
