#include"globedef.h"

#include "ChinaMarket.h"

CRTDataPtr gl_TESTpRTData = nullptr;// 测试用变量

CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在第一位，其他全局变量初始化时用到此变量。

CThreadStatus gl_ThreadStatus; // 系统中的各种状态，被各个工作线程所使用

atomic_bool gl_ExitingSystem = false; //  系统退出标识，用于终止其他线程。
bool gl_fExitingCalculatingRS = false; // 用于通知工作线程退出的信号

bool gl_fNormalMode = false; // 测试模式标识，默认为真。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。
bool gl_fTestMode = true; // 是否设置了gl_fTestMode标识（用于判断是否在实际系统中使用了MySQLTest驱动）。

vector<CVirtualMarketPtr> gl_vMarketPtr; // 各市场指针的容器，只用于执行各市场的SchedulingTask

#ifdef __GOOGLEMOCK__
#include"MockChinaMarket.h"
using namespace Testing;

// 各市场。皆为唯一实例
CPotenDailyBriefingMarketPtr gl_pPotenDailyBriefingMarket = nullptr;
CCrweberIndexMarketPtr gl_pCrweberIndexMarket = nullptr;
CMockChinaMarketPtr gl_pChinaStockMarket = nullptr;  // 此市场实例必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。

#else
// 各市场。皆为唯一实例
CPotenDailyBriefingMarketPtr gl_pPotenDailyBriefingMarket = nullptr;
CCrweberIndexMarketPtr gl_pCrweberIndexMarket = nullptr;
CChinaMarketPtr gl_pChinaStockMarket = nullptr;  // 此市场实例必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。

#endif