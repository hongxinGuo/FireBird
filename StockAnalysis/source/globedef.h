#ifndef __GLOBE_DEFINE__
#define __GLOBE_DEFINE__

#include"stdafx.h"

#include"RTData.h"

#include"SetDayLine.h"
#include"StockID.h"

#include"SystemMessage.h"
#include"SystemDequeData.h"
#include"SystemStatus.h"
#include"SystemTime.h"

using namespace std;
#include<deque>
#include<vector>
#include<map>
#include<deque>
#include<memory>

extern CSystemMessage     gl_systemMessage;     // 系统消息汇总类。此变量必须放在第一位，其他全局变量初始化时用到此变量。
extern CSystemTime        gl_systemTime;        // 系统时间汇总。
extern CSystemQueueData   gl_systemDequeData;   // 系统中的各种队列，被各个工作线程使用。
extern CSystemStatus      gl_systemStatus;      // 系统中的各种状态，被各个工作线程所使用。

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

extern DayLineInquire gl_stDayLineInquire; 

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

extern RTDataInquire gl_stRTDataInquire;

extern bool gl_fExiting; //  系统退出标识，用于终止其他线程。
extern bool gl_fExitingCalculatingRelativeStrong;// 用于通知工作线程退出的信号
extern bool gl_fResetSystem; // 系统重启标识

extern bool gl_fTestMode;               // 测试模式标识，默认为真。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。
extern bool gl_fInTestMode;             // 是否设置了gl_fTestMode标识（用于判断是否在实际系统中使用了MySQLTest驱动）。

class CMarket;
// 此市场变量必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。
extern CMarket gl_ChinaStockMarket;        // 市场。所有活跃的股票皆位于其中，单一实例变量，仅允许存在一个实例。

#endif
