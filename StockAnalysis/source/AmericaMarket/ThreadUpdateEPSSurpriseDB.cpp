#include"globedef.h"

UINT ThreadUpdateEPSSurpriseDB(not_null<CAmericaStakePtr> pStake) {
  gl_SaveEPSSurprise.Wait();
  pStake->UpdateEPSSurpriseDB();
  gl_ThreadStatus.DecreaseSavingDayLineThreads();
  gl_SaveEPSSurprise.Signal();

  return 41;
}