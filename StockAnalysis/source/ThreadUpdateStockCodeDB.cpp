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
UINT ThreadUpdateStockCodeDB(CChinaMarket* pMarket) {
  //
  gl_ThreadStatus.IncreaseRunningThread();
  pMarket->UpdateStockCodeDB();
  gl_ThreadStatus.DecreaseRunningThread();
  return 18;
}

UINT ThreadUpdateOptionDB(CChinaMarket* pMarket) {
  gl_ThreadStatus.IncreaseRunningThread();
  pMarket->UpdateOptionDB();
  gl_ThreadStatus.DecreaseRunningThread();
  return 20;
}

UINT ThreadSavePotenData(CPotenDailyBriefingMarket* pMarket) {
  gl_ThreadStatus.IncreaseRunningThread();
  pMarket->SaveCurrentData();
  gl_ThreadStatus.DecreaseRunningThread();
  return 21;
}

UINT ThreadAppendChoicedStockDB(CChinaMarket* pMarket) {
  gl_ThreadStatus.IncreaseRunningThread();
  pMarket->AppendChoicedStockDB();
  gl_ThreadStatus.DecreaseRunningThread();
  return 22;
}