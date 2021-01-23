#include"globedef.h"

UINT ThreadUpdateForexSymbolDB(not_null<CWorldMarket*> pMarket) {
  gl_UpdateWorldMarketDB.Wait();
  pMarket->UpdateForexSymbolDB();
  gl_UpdateWorldMarketDB.Signal();

  return 39;
}