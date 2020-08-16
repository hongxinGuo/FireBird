///////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"Thread.h"

#include"ChinaMarket.h"

using namespace std;
#include<thread>

UINT ThreadBuildWeekLine(CChinaMarket* pMarket, long lStartDay) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_ThreadStatus.SetCreateWeekLine(true);

  long lStartMonday = GetCurrentMonday(lStartDay);

  if (lStartDay > 19900101) { // 此种情况只用于重新生成本周周线
    ASSERT(lStartMonday == GetCurrentMonday(pMarket->GetFormatedMarketDay()));
    pMarket->DeleteWeekLine(lStartMonday);
  }

  pMarket->BuildWeekLine(lStartMonday);

  gl_ThreadStatus.SetCreateWeekLine(false);
  gl_ThreadStatus.DecreaseRunningThread();

  return 25;
}

UINT ThreadBuildWeekLineOfStock(CChinaStockPtr pStock, long lStartDay) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_SemaphoreBackGroundTaskThreads.Wait();
  gl_ThreadStatus.IncreaseBackGroundWorkingthreads();
  if (!gl_fExitingSystem) pStock->BuildWeekLine(lStartDay);

  gl_ThreadStatus.DecreaseBackGroundWorkingthreads();
  gl_SemaphoreBackGroundTaskThreads.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 26;
}

UINT ThreadBuildWeekLineOfCurrentWeek(CChinaMarket* pMarket) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_SemaphoreBackGroundTaskThreads.Wait();
  gl_ThreadStatus.IncreaseBackGroundWorkingthreads();

  pMarket->BuildWeekLineOfCurrentWeek();

  gl_ThreadStatus.DecreaseBackGroundWorkingthreads();
  gl_SemaphoreBackGroundTaskThreads.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 32;
}