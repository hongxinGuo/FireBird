#include"globedef.h"

UINT ThreadUpdateStockDB(not_null<CWorldMarket*> pMarket) {
  gl_UpdateWorldMarketDB.Wait();
  pMarket->UpdateStockDB();
  gl_UpdateWorldMarketDB.Signal();

  return 37;
}