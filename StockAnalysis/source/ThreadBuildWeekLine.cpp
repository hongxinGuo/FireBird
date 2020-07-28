///////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"Thread.h"

#include"ChinaMarket.h"

using namespace std;
#include<thread>

UINT ThreadBuildWeekLine(CChinaMarket* pMarket) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_ThreadStatus.SetCreateWeekLine(true);

  pMarket->BuildWeekLine();

  gl_ThreadStatus.SetCreateWeekLine(false);
  gl_ThreadStatus.DecreaseRunningThread();

  return 25;
}

UINT ThreadBuildWeekLineOfStock(CChinaStockPtr pStock) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_SemaphoreBackGroundTaskThreads.Wait();
  gl_ThreadStatus.IncreaseBackGroundWorkingthreads();
  if (!gl_fExitingSystem) pStock->BuildWeekLine();

  gl_ThreadStatus.DecreaseBackGroundWorkingthreads();
  gl_SemaphoreBackGroundTaskThreads.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 26;
}

UINT ThreadBuildWeekLineRS(CChinaMarket* pMarket) {
  pMarket->BuildWeekLineRS();

  return 27;
}

UINT ThreadBuildWeekLineRSOfStock(CChinaStockPtr pStock) {
  gl_ThreadStatus.IncreaseRunningThread();
  gl_SemaphoreBackGroundTaskThreads.Wait();
  gl_ThreadStatus.IncreaseBackGroundWorkingthreads();

  pStock->BuildWeekLineRS();

  gl_ThreadStatus.DecreaseBackGroundWorkingthreads();
  gl_SemaphoreBackGroundTaskThreads.Signal();
  gl_ThreadStatus.DecreaseRunningThread();

  return 28;
}