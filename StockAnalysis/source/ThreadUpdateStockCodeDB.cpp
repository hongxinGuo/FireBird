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
UINT ThreadUpdateStockCodeDB(void) {
  //
  gl_pChinaStockMarket->UpdateStockCodeDB();
  return 18;
}

UINT ThreadUpdateOptionDB(void) {
  gl_pChinaStockMarket->UpdateOptionDB();
  return 20;
}

UINT ThreadSavePotenData(void) {
  gl_pPotenDailyBriefingMarket->SaveCurrentData();
  return 21;
}