#include"globedef.h"

#include"DayLine.h"
#include"ChinaMarket.h"

#include"Thread.h"

//////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
UINT ThreadUpdateStakeCodeDB(not_null<CChinaMarket*> pMarket) {
  //
  gl_ThreadStatus.IncreaseRunningThread();
  pMarket->UpdateStakeCodeDB();
  gl_ThreadStatus.DecreaseRunningThread();
  return 18;
}

UINT ThreadUpdateOptionDB(not_null<CChinaMarket*> pMarket) {
  gl_ThreadStatus.IncreaseRunningThread();
  pMarket->UpdateOptionDB();
  gl_ThreadStatus.DecreaseRunningThread();
  return 20;
}

UINT ThreadSavePotenData(not_null<CPotenDailyBriefingMarket*> pMarket) {
  gl_ThreadStatus.IncreaseRunningThread();
  pMarket->SaveCurrentData();
  gl_ThreadStatus.DecreaseRunningThread();
  return 21;
}

UINT ThreadAppendChoicedStockDB(not_null<CChinaMarket*> pMarket) {
  gl_ThreadStatus.IncreaseRunningThread();
  pMarket->AppendChoicedStockDB();
  gl_ThreadStatus.DecreaseRunningThread();
  return 22;
}