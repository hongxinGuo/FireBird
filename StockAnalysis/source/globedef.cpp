#include"globedef.h"

#include"StockID.h"
#include "Market.h"
#include "SystemTime.h"

CSystemTime gl_sTime;

vector<StockIDPtr>		gl_vTotalStock;
map<CString, long>		gl_mapTotalStockToIndex;		// 将所有被查询的股票代码映射为偏移量（目前只接受A股信息）

vector<StockIDPtr> gl_vStockChoice;   // 自选股票池

CSetDayLine gl_setSavingDayLineOnly; // 此变量专用于存储接收到的日线历史数据。永远打开状态，否则当同时操作数据库时速度奇慢。

CSystemTime       gl_systemTime;            // 系统时间汇总。
CSystemMessage    gl_systemMessage;         // 系统消息汇总类
CSystemDequeData  gl_systemDequeData;       // 系统中的各种队列，被各个工作线程使用。
CSystemStatus     gl_systemStatus;          // 系统中的各种状态，被各个工作线程所使用。

deque<CString> gl_dequeRTStockInquire; // 申请实时股票信息的股票队列
deque<CString> gl_dequeDayLineStockInquire; // 申请日线历史数据的股票队列。

DayLineInquire gl_stDayLineInquire;
RTDataInquire gl_stRTDataInquire;

bool gl_fExiting = false; //  系统退出标识，用于终止其他线程。
bool gl_fExitingCalculatingRelativeStrong = false;

long gl_lShowGuaDanTransaction = __ORDINARY_BUY__; // 显示挂单成交具体情况

CMarket gl_ChinaStockMarket; // 此市场实例必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。

