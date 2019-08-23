#include"globedef.h"

#include"StockID.h"
#include "Market.h"
#include "SystemTime.h"

CSystemMessage    gl_systemMessage;         // 系统消息汇总类。此变量必须放在第一位，其他全局变量初始化时用到此变量。
CSystemTime       gl_systemTime;            // 系统时间汇总。
CSystemQueueData  gl_systemDequeData;       // 系统中的各种队列，被各个工作线程使用。
CThreadStutus     gl_ThreadStatus;          // 系统中的各种状态，被各个工作线程所使用

DayLineInquire gl_stDayLineInquire;
RTDataInquire gl_stRTDataInquire;

bool gl_fExiting = false; //  系统退出标识，用于终止其他线程。
bool gl_fExitingCalculatingRelativeStrong = false; // 用于通知工作线程退出的信号
bool gl_fResetSystem = false; // 系统重启标识

bool gl_fNormalMode = false;               // 测试模式标识，默认为真。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。
bool gl_fInTestMode = true;             // 是否设置了gl_fTestMode标识（用于判断是否在实际系统中使用了MySQLTest驱动）。

CMarket gl_ChinaStockMarket; // 此市场实例必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。
