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

class CMarket;


extern vector<StockIDPtr>	gl_vTotalStock;             // 本系统允许的所有股票池（无论代码是否存在）
extern map<CString, long>	gl_mapTotalStockToIndex;		// 将所有被查询的股票代码映射为偏移量（目前只接受A股信息）

extern vector<StockIDPtr> gl_vStockChoice;   // 自选股票池


extern CSetDayLine gl_setSavingDayLineOnly; // 此变量专用于存储接收到的日线历史数据。永远打开状态，否则当同时操作数据库时速度奇慢。

extern CSystemTime        gl_systemTime;         // 系统时间汇总。
extern CSystemMessage     gl_systemMessage;      // 系统消息汇总类，被各个工作线程所使用
extern CSystemDequeData   gl_systemDequeData;    // 系统中的各种队列，被各个工作线程使用。
extern CSystemStatus      gl_systemStatus;       // 系统中的各种状态，被各个工作线程所使用。

extern deque<CString> gl_dequeRTStockInquire; // 申请实时股票信息的股票队列
extern deque<CString> gl_dequeDayLineStockInquire; // 申请日线历史数据的股票队列。

typedef struct stDayLine {    // 日线数据结构
  stDayLine() noexcept {
    lByteRead = 0;
    fError = false;
    strInquire = "";
  }

  CString strInquire;   // 申请提取数据的字符串
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

extern bool gl_fExiting;
extern bool gl_fExitingCalculatingRelativeStrong;

extern long gl_lShowGuaDanTransaction; // 显示挂单成交具体情况

// 此市场变量必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。
extern CMarket gl_ChinaStockMarket;        // 市场。所有活跃的股票皆位于其中，单一实例变量，仅允许存在一个实例。

#endif
