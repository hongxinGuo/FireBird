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
  gl_ThreadStatus.IncreaseNumberOfRunningThread();
  pMarket->UpdateStockCodeDB();
  gl_ThreadStatus.DecreaseNumberOfRunningThread();
  return 18;
}

UINT ThreadUpdateOptionDB(CChinaMarket* pMarket) {
  gl_ThreadStatus.IncreaseNumberOfRunningThread();
  pMarket->UpdateOptionDB();
  gl_ThreadStatus.DecreaseNumberOfRunningThread();
  return 20;
}

UINT ThreadSavePotenData(CPotenDailyBriefingMarket* pMarket) {
  gl_ThreadStatus.IncreaseNumberOfRunningThread();
  pMarket->SaveCurrentData();
  gl_ThreadStatus.DecreaseNumberOfRunningThread();
  return 21;
}