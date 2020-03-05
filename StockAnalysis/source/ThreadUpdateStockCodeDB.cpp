#include"globedef.h"

#include"DayLine.h"
#include"ChinaMarket.h"

#include"Thread.h"
#include"TransferSharedPtr.h"

//////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
UINT ThreadUpdateStockCodeDB(void) {
  //
  gl_ChinaStockMarket.UpdateStockCodeDB();
  return 18;
}

UINT ThreadUpdateOptionDB(void) {
  gl_ChinaStockMarket.UpdateOptionDB();
  return 20;
}

UINT ThreadSavePotenData(void) {
  gl_PotenDailyBriefingMarket.SaveCurrentData();

  return 21;
}