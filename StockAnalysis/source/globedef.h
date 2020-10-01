#pragma once

// 中国股票市场起始日
#define __CHINA_MARKET_BEGIN_DATE__ 19900101

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

#include"stdafx.h"

#include"VirtualMarket.h"
#include"PotenDailyBriefingMarket.h"
#include"CrweberIndexMarket.h"
#include"ChinaMarket.h"

#include"WebRTData.h"

#include"SystemMessage.h"
#include"WebRTDataContainer.h"
#include"QueueWebData.h"
#include"PriorityQueueWebRTData.h"
#include"ThreadStatus.h"

using namespace std;
#include<deque>
#include<atomic>
#include<vector>

extern CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在第一位，其他全局变量初始化时用到此变量。

extern CThreadStatus gl_ThreadStatus; // 系统中的各种状态，被各个工作线程所使用

extern deque<CString> gl_dequeRTStockInquire; // 申请实时股票信息的股票队列（尚未使用）
extern deque<CString> gl_dequeDLStockInquire; // 申请日线历史数据的股票队列（尚未使用）

extern atomic_bool gl_fExitingSystem; //  系统退出标识，用于终止其他线程。
extern bool gl_fExitingCalculatingRS; // 用于通知工作线程退出的信号

extern bool gl_fNormalMode; // 正常模式标识，默认为假。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。
extern bool gl_fTestMode; // 是否设置了gl_fTestMode标识，默认为真。系统在启动时需要设置此标识为假。（用于判断是否在实际系统中使用了MySQLTest驱动）。

extern vector<CVirtualMarketPtr> gl_vMarketPtr; // 各市场指针的容器，只用于执行各市场的SchedulingTask

// 各市场唯一的实例
extern CPotenDailyBriefingMarketPtr gl_pPotenDailyBriefingMarket;
extern CCrweberIndexMarketPtr gl_pCrweberIndexMarket;
// 此市场变量必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。
extern CChinaMarketPtr gl_pChinaStockMarket; // 市场。所有活跃的股票皆位于其中，单一实例变量，仅允许存在一个实例。
