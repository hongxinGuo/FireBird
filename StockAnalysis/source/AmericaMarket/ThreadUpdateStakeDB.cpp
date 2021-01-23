#include"globedef.h"

UINT ThreadUpdateStakeDB(not_null<CWorldMarket*> pMarket) {
  gl_UpdateWorldMarketDB.Wait();
  pMarket->UpdateStakeDB3();
  gl_UpdateWorldMarketDB.Signal();

  return 37;
}