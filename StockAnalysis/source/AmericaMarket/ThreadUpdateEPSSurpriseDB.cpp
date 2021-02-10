#include"globedef.h"

UINT ThreadUpdateEPSSurpriseDB(not_null<CWorldStock*> pStock) {
  gl_UpdateWorldMarketDB.Wait();
  pStock->UpdateEPSSurpriseDB();
  gl_UpdateWorldMarketDB.Signal();

  return 41;
}