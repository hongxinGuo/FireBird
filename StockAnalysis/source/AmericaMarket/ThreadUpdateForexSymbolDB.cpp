#include"globedef.h"

UINT ThreadUpdateForexSymbolDB(not_null<CAmericaMarket*> pMarket) {
  gl_UpdateAmericaMarketDB.Wait();
  pMarket->UpdateForexSymbolDB();
  gl_UpdateAmericaMarketDB.Signal();

  return 39;
}