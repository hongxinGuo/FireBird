#ifndef __GLOBE_DEFINE__
#define __GLOBE_DEFINE__

#include"stdafx.h"

#include"RTData.h"
#include"SetDayLine.h"
#include"StockID.h"

#include"SystemMessage.h"
#include"SystemDequeData.h"

using namespace std;
#include<deque>
#include<vector>
#include<map>
#include<deque>
#include<memory>

class CMarket;

class CStock;

extern CString gl_sCurrentBaseDir;
extern CString gl_sDataBaseDir;
extern CString gl_sDataBaseName;

extern CMarket gl_sMarket;        // 市场。所有活跃的股票皆位于其中，单一实例变量，仅允许存在一个实例。


extern CString gl_strRTStockSource;
extern DWORD	 gl_dwRTDataHttpFileStatus;

extern CString gl_strDayLineStockSource;
extern CString gl_strDayLinePostfix;
extern DWORD	 gl_dwDayLineHttpFileStatus;

extern CString	gl_strCurrentStockDownLoading; // 目前正在下载日线历史数据的股票代码

extern vector<StockIDPtr>	gl_vTotalStock;             // 本系统允许的所有股票池（无论代码是否存在）
extern map<CString, long>	gl_mapTotalStockToIndex;		// 将所有被查询的股票代码映射为偏移量（目前只接受A股信息）

extern vector<StockIDPtr> gl_vStockChoice;   // 自选股票池

extern long gl_lToday;          // 今日日期。格式为：19990102
extern long gl_lLastTradeDay; // 上一个交易日，用于判断是否申请日线数据。日线数据的最新日，采用上一个交易日，本交易日的数据由实时数据产生。
extern long gl_lTime;				  // 今日时间。格式为：hhmmss
extern tm gl_tm;						  // 全局时间。由CMainFrame的OnTimer函数负责更新
extern time_t gl_ttime;       // 全局时间。由CMainFrame的OnTimer函数负责更新
extern bool gl_fMarketOpened; // 市场是否处于交易时段

extern long gl_lRelativeStrongStartDay;
extern long gl_lRelativeStrongEndDay;

extern CSetDayLine gl_setSavingDayLineOnly; // 此变量专用于存储接收到的日线历史数据。

extern CSystemMessage gl_systemMessage;       // 系统消息汇总类，被各个工作线程所使用

extern CSystemDequeData gl_systemDequeData;    // 系统中的各种队列，被各个工作线程使用。

extern deque<CString> gl_dequeRTStockInquire; // 申请实时股票信息的股票队列
extern deque<CString> gl_dequeDayLineStockInquire; // 申请日线历史数据的股票队列。

typedef struct stDayLine {    // 日线数据结构
  stDayLine() {
    fDataReady = false;
    fReadingInProcess = false;
    fDataBaseInProcess = false;
    fCalculatingRelativeStrongInProcess = false;
    lByteRead = 0;
    fError = false;
    strInquire = "";
  }
  CString strInquire;   // 申请提取数据的字符串
  bool fDataReady;      // 数据已提取完成标识
  bool fReadingInProcess; // 是否处于提取中标识
  bool fDataBaseInProcess; // 是否处于存储日线历史数据的数据库操作中。
  bool fCalculatingRelativeStrongInProcess; // 是否处于计算相对强度的过程中标识
  char buffer[2048 * 1024]; // 提取到的日线历史数据缓存器
  long lByteRead;           // 本次读到的字节数
  bool fError;              // 本次处理日线数据是否有误的标识
} DayLineInquire;

extern DayLineInquire gl_stDayLineInquire; 

typedef struct stRTData {
  stRTData() {
    fDataReady = false;
    fReceiveFromWebInProcess = false;
    lByteRead = 0;
    fError = false;
    strInquire = "";
  }
  CString strInquire;
  bool fDataReady;
  bool fReceiveFromWebInProcess;  // 接收实时数据线程是否执行标识
  char buffer[2048 * 1024];
  long lByteRead;
  bool fError;
} RTDataInquire;

extern RTDataInquire gl_stRTDataInquire;

extern bool gl_fExiting;

extern long gl_lOption_CalculatingRelativeStrongStartDay;
extern bool gl_fExitingCalculatingRelativeStrong;
#endif
