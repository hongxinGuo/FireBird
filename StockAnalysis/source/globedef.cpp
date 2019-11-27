#include"globedef.h"

#include "Market.h"
#include "SystemTime.h"

// 测试用变量
CRTDataPtr gl_TESTpRTData = nullptr;

CSinaRTWebData gl_SinaRTWebData; // 新浪实时数据采集
CTengxunRTWebData gl_TengxunRTWebData; // 腾讯实时数据采集
CNeteaseRTWebData gl_NeteaseRTWebData; // 网易实时数据采集
CNeteaseDayLineWebData gl_NeteaseDayLineWebData; // 网易日线历史数据
CNeteaseDayLineWebData gl_NeteaseDayLineWebDataSecond; // 网易日线历史数据
CNeteaseDayLineWebData gl_NeteaseDayLineWebDataThird; // 网易日线历史数据
CNeteaseDayLineWebData gl_NeteaseDayLineWebDataFourth; // 网易日线历史数据
CNeteaseDayLineWebData gl_NeteaseDayLineWebDataFive; // 网易日线历史数据
CNeteaseDayLineWebData gl_NeteaseDayLineWebDataSix; // 网易日线历史数据
CCrweberIndexWebData gl_CrweberIndexWebData; // crweber.com上的每日油运指数

CCrweberIndex gl_CrweberIndex;
CCrweberIndex gl_CrweberIndexLast;

CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在第一位，其他全局变量初始化时用到此变量。
CSystemTime gl_systemTime; // 系统时间汇总。
CQueueRTData gl_QueueSinaRTData; // 系统实时数据队列。
CQueueRTData gl_QueueTengxunRTData; // 系统实时数据队列。
CThreadStatus gl_ThreadStatus; // 系统中的各种状态，被各个工作线程所使用

atomic<bool> gl_ExitingSystem = false; //  系统退出标识，用于终止其他线程。
bool gl_fExitingCalculatingRS = false; // 用于通知工作线程退出的信号
bool gl_fResetSystem = false; // 系统重启标识

bool gl_fNormalMode = false; // 测试模式标识，默认为真。系统需要在启动时设置此标识，否则只有读取数据库的权利，无法添加和更改。
bool gl_fTestMode = true; // 是否设置了gl_fTestMode标识（用于判断是否在实际系统中使用了MySQLTest驱动）。

const int gl_cMaxSavingOneDayLineThreads = 3;
const int gl_cMaxCalculateDayLineRS = 8;
// 信号量必须声明为全局变量（为了初始化）
CSemaphore gl_SaveOneStockDayLine(gl_cMaxSavingOneDayLineThreads, gl_cMaxSavingOneDayLineThreads);
CSemaphore gl_CalculateDayLineRelativeStrong(gl_cMaxCalculateDayLineRS, gl_cMaxCalculateDayLineRS); // 计算日线相对强度的线程最多允许8个同时运行
CSemaphore gl_ProcessSinaRTDataQueue(1, 1);   // 新浪实时数据处理同时只允许一个线程存在
CSemaphore gl_ProcessTengxunRTDataQueue(1, 1);
CSemaphore gl_ProcessNeteaseRTDataQueue(1, 1);

CMarket gl_ChinaStockMarket;  // 此市场实例必须位于全局变量的最后，因为其初始化需要其他全局变量的支持。