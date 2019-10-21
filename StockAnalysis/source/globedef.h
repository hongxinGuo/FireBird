#ifndef __GLOBE_DEFINE__
#define __GLOBE_DEFINE__

#include"stdafx.h"

#include"RTData.h"

#include"SetDayLine.h"
#include"StockID.h"

#include"SinaRTWebData.h"
//#include"TengxunRTWebData.h"

#include"SystemMessage.h"
#include"QueueRTData.h"
#include"ThreadStatus.h"
#include"SystemTime.h"

using namespace std;
#include<deque>
#include<vector>
#include<map>
#include<deque>
#include<memory>

extern CSinaRTWebData     gl_SinaRTWebData;     // 新浪实时数据采集
//extern CTengxunRTWebData  gl_TengxunRTWebData;  // 腾讯实时数据采集

extern CSystemMessage     gl_systemMessage;     // 系统消息汇总类。此变量必须放在第一位，其他全局变量初始化时用到此变量。
extern CSystemTime        gl_systemTime;        // 系统时间汇总。
extern CQueueRTData       gl_QueueRTData;       // 系统实时数据队列。
extern CThreadStatus      gl_ThreadStatus;      // 系统中的各种状态，被各个工作线程所使用

extern deque<CString> gl_dequeRTStockInquire;   // 申请实时股票信息的股票队列
extern deque<CString> gl_dequeDayLineStockInquire; // 申请日线历史数据的股票队列。

typedef struct stDayLine {    // 日线数据结构
  stDayLine() noexcept {
    lByteRead = 0;
    fError = false;
    strInquire = "";
  }

  CString strInquire;       // 申请提取数据的字符串
  char buffer[2048 * 1024]; // 提取到的日线历史数据缓存器
  long lByteRead;           // 本次读到的字节数
  bool fError;              // 本次处理日线数据是否有误的标识

protected:
} DayLineInquire;

extern DayLineInquire gl_stDayLineInquire;      // 网易日线历史数据缓存

typedef struct stRTData {
  stRTData() noexcept {
    lByteRead = 0;
    fError = false;
    strInquire = "";
  }
  CString strInquire;
  char buffer[2048 * 1024];
  long lByteRead;
  bool fError;
} RTDataInquire;

//extern RTDataInquire gl_stSinaRTDataInquire;    // 新浪实时行情数据缓存
extern RTDataInquire gl_stTengxunRTDataInquire; // 腾讯实时行情数据缓存

extern bool gl_fExiting;                        //  系统退出标识，用于终止其他线程。
extern bool gl_fExitingCalculatingRS;           // 用于通知工作线程退出的信号
extern bool gl_fResetSystem;                    // 系统重启标识

extern bool gl_fNormalMode;                     // 正常模式标识，默认为假。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。
extern bool gl_fTestMode;                       // 是否设置了gl_fTestMode标识（用于判断是否在实际系统中使用了MySQLTest驱动）。

class CMarket;
// 此市场变量必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。
extern CMarket gl_ChinaStockMarket;        // 市场。所有活跃的股票皆位于其中，单一实例变量，仅允许存在一个实例。

#endif
