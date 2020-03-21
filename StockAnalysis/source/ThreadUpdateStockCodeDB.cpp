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
  pMarket->UpdateStockCodeDB();
  return 18;
}

UINT ThreadUpdateOptionDB(CChinaMarket* pMarket) {
  pMarket->UpdateOptionDB();
  return 20;
}

UINT ThreadSavePotenData(CPotenDailyBriefingMarket* pMarket) {
  pMarket->SaveCurrentData();
  return 21;
}