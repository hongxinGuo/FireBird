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
UINT ThreadUpdateStockCodeDB(LPVOID) {
  //
  gl_ChinaStockMarket.UpdateStockCodeDB();
  return 18;
}

UINT ThreadUpdateOptionDB(LPVOID) {
  gl_ChinaStockMarket.UpdateOptionDB();
  return 20;
}

UINT ThreadSavePotenData(LPVOID) {
  gl_PotenDailyBriefingMarket.SaveCurrentData();

  return 21;
}