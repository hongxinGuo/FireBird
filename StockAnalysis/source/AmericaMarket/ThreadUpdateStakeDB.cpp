#include"globedef.h"

UINT ThreadUpdateStakeDB(not_null<CAmericaMarket*> pMarket) {
  gl_SaveAmericaOneStockDayLine.Wait();
  gl_SaveEPSSurprise.Wait();
  pMarket->UpdateStakeDB();
  gl_SaveEPSSurprise.Signal();
  gl_SaveAmericaOneStockDayLine.Signal();

  return 37;
}