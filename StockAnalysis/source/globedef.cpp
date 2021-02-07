#include"globedef.h"

#include "ChinaMarket.h"

CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在第一位，其他全局变量初始化时用到此变量。

CThreadStatus gl_ThreadStatus; // 系统中的各种状态，被各个工作线程所使用

deque<CString> gl_dequeRTStockInquire; // 申请实时股票信息的股票队列（尚未使用）
deque<CString> gl_dequeDayLineStockInquire; // 申请日线历史数据的股票队列（尚未使用）

atomic_bool gl_fExitingSystem = false; //  系统退出标识，用于终止其他线程。
bool gl_fExitingCalculatingRS = false; // 用于通知工作线程退出的信号

bool gl_fNormalMode = false; // 测试模式标识，默认为真。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。
bool gl_fTestMode = true; // 是否设置了gl_fTestMode标识（用于判断是否在实际系统中使用了MySQLTest驱动）。

vector<CVirtualMarketPtr> gl_vMarketPtr; // 各市场指针的容器，只用于执行各市场的SchedulingTask

// 各市场。皆为唯一实例
CWorldMarketPtr gl_pWorldMarket = nullptr;
CPotenDailyBriefingMarketPtr gl_pPotenDailyBriefingMarket = nullptr;
CCrweberIndexMarketPtr gl_pCrweberIndexMarket = nullptr;
CChinaMarketPtr gl_pChinaStockMarket = nullptr;  // 此市场实例必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。
