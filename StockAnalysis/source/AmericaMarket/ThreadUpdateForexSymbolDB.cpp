#include"globedef.h"

UINT ThreadUpdateForexSymbolDB(not_null<CAmericaStakeMarket*> pMarket) {
  gl_SaveAmericaOneStockDayLine.Wait();
  pMarket->UpdateForexSymbolDB();
  gl_SaveAmericaOneStockDayLine.Signal();

  return 39;
}