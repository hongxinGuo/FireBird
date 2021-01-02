#include"globedef.h"

UINT ThreadUpdateStakeDB(not_null<CAmericaMarket*> pMarket) {
  gl_SaveAmericaStockDayLine.Wait();
  gl_SaveEPSSurprise.Wait();
  pMarket->UpdateStakeDB();
  gl_SaveEPSSurprise.Signal();
  gl_SaveAmericaStockDayLine.Signal();

  return 37;
}