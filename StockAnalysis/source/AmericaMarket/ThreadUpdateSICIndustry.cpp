#include"pch.h"
#include"globedef.h"

UINT ThreadUpdateSICIndustry(not_null<CWorldMarket*> pMarket) {
  gl_ThreadStatus.IncreaseSavingThread();
  gl_UpdateWorldMarketDB.Wait();
  pMarket->UpdateSICIndustry();
  gl_UpdateWorldMarketDB.Signal();
  gl_ThreadStatus.DecreaseSavingThread();

  return 46;
}