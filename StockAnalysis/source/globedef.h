#pragma once

#include"stdafx.h"

// 中国股票市场起始日
#define __CHINA_MARKET_BEGIN_DAY__ 19900101

enum {
  __STOCK_NOT_CHECKED__ = 128, // 尚未检查过的股票代码
  __STOCK_IPOED__ = 255, // 正常股票标识
  __STOCK_NULL__ = 0, // 无效代码（此代码的股票不存在）
  __STOCK_DELISTED__ = 1, // 已退市（或停牌）的股票标识
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

#include"VirtualMarket.h"
#include"PotenDailyBriefMarket.h"

#include"RTData.h"

#include"SystemMessage.h"
#include"QueueRTData.h"
#include"QueueWebData.h"
#include"PriorityQueueRTData.h"
#include"ThreadStatus.h"
#include"SystemTime.h"

using namespace std;
#include<deque>
#include<atomic>
#include<vector>

// 测试用变量
extern CRTDataPtr gl_TESTpRTData;

extern CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在第一位，其他全局变量初始化时用到此变量。
extern CSystemTime gl_systemTime; // 系统时间汇总。

extern CThreadStatus gl_ThreadStatus; // 系统中的各种状态，被各个工作线程所使用

extern deque<CString> gl_dequeRTStockInquire; // 申请实时股票信息的股票队列
extern deque<CString> gl_dequeDayLineStockInquire; // 申请日线历史数据的股票队列。

extern atomic_bool gl_ExitingSystem; //  系统退出标识，用于终止其他线程。
extern bool gl_fExitingCalculatingRS; // 用于通知工作线程退出的信号
extern bool gl_fResetSystem; // 系统重启标识

extern bool gl_fNormalMode; // 正常模式标识，默认为假。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。
extern bool gl_fTestMode; // 是否设置了gl_fTestMode标识（用于判断是否在实际系统中使用了MySQLTest驱动）。

extern const int gl_cMaxSavingOneDayLineThreads;

extern vector<CVirtualMarket*> gl_vMarket; // 各市场指针的容器，只用于执行各市场的SchedulingTask

extern CPotenDailyBriefingMarket gl_PotenDailyBriefingMarket;

class CChinaMarket;
// 此市场变量必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。
extern CChinaMarket gl_ChinaStockMarket; // 市场。所有活跃的股票皆位于其中，单一实例变量，仅允许存在一个实例。
