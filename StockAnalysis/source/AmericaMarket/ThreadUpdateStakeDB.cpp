#include"globedef.h"

UINT ThreadUpdateStakeDB(not_null<CAmericaMarket*> pMarket) {
  gl_UpdateAmericaMarketDB.Wait();
  pMarket->UpdateStakeDB();
  gl_UpdateAmericaMarketDB.Signal();

  return 37;
}