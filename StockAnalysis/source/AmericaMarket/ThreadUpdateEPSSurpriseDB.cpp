#include"globedef.h"

UINT ThreadUpdateEPSSurpriseDB(not_null<CAmericaStake*> pStake) {
  gl_UpdateAmericaMarketDB.Wait();
  pStake->UpdateEPSSurpriseDB();
  gl_UpdateAmericaMarketDB.Signal();

  return 41;
}