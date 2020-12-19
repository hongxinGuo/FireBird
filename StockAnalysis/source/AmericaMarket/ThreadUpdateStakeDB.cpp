#include"globedef.h"

UINT ThreadUpdateStakeDB(not_null<CAmericaMarket*> pMarket) {
  gl_SaveAmericaOneStockDayLine.Wait();
  pMarket->UpdateStakeDB();
  gl_SaveAmericaOneStockDayLine.Signal();

  return 37;
}