#include"globedef.h"

UINT ThreadUpdateEPSSurpriseDB(not_null<CAmericaStake*> pStake) {
  gl_SaveEPSSurprise.Wait();
  pStake->UpdateEPSSurpriseDB();
  gl_SaveEPSSurprise.Signal();

  return 41;
}