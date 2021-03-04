#include"pch.h"
#include"globedef.h"

UINT ThreadUpdateStockDB(not_null<CWorldMarket*> pMarket) {
  gl_ThreadStatus.IncreaseSavingThread();
  gl_UpdateWorldMarketDB.Wait();
  pMarket->UpdateStockDB();
  gl_UpdateWorldMarketDB.Signal();
  gl_ThreadStatus.DecreaseSavingThread();

  return 37;
}