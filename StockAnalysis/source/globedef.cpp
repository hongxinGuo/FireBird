#include"globedef.h"

#include"StockID.h"
#include "Stock.h"
//#include "RTData.h"
#include "Market.h"
#include "SystemTime.h"
#include "MutualLock.h"

CMarket gl_ChinaStockMarket;

CSystemTime gl_sTime;

CMutualLock gl_sMutualLock;

vector<StockIDPtr>		gl_vTotalStock;
map<CString, long>		gl_mapTotalStockToIndex;		// 将所有被查询的股票代码映射为偏移量（目前只接受A股信息）

vector<StockIDPtr> gl_vStockChoice;   // 自选股票池

long gl_lToday;           // 今日日期。格式为：19990102
long gl_lLastTradeDay;  // 上一个交易日，用于判断是否申请日线数据。日线数据的最新日，采用上一个交易日，本交易日的数据由实时数据产生。
long gl_lTime;				  // 今日时间。格式为：hhmmss
tm gl_tm;						    // 全局时间。由CMainFrame的OnTimer函数负责更新
time_t gl_ttime;        // 全局时间。由CMainFrame的OnTimer函数负责更新

CSetDayLine gl_setSavingDayLineOnly; // 此变量专用于存储接收到的日线历史数据。

CSystemMessage gl_systemMessage;       // 系统消息汇总类
CSystemDequeData gl_systemDequeData;    // 系统中的各种队列，被各个工作线程使用。
CSystemStatus gl_systemStatus;         // 系统中的各种状态，被各个工作线程所使用。

deque<CString> gl_dequeRTStockInquire; // 申请实时股票信息的股票队列
deque<CString> gl_dequeDayLineStockInquire; // 申请日线历史数据的股票队列。

DayLineInquire gl_stDayLineInquire;
RTDataInquire gl_stRTDataInquire;

bool gl_fExiting = false; //  系统退出标识，用于终止其他线程。



