#include"globedef.h"

UINT ThreadUpdateStakeDB(not_null<CAmericaMarket*> pMarket) {
  gl_UpdateAmericaMarketDB.Wait();
  pMarket->UpdateStakeDB3();
  gl_UpdateAmericaMarketDB.Signal();

  return 37;
}