#include"globedef.h"

UINT ThreadUpdateEPSSurpriseDB(not_null<CWorldStock*> pStake) {
  gl_UpdateWorldMarketDB.Wait();
  pStake->UpdateEPSSurpriseDB();
  gl_UpdateWorldMarketDB.Signal();

  return 41;
}