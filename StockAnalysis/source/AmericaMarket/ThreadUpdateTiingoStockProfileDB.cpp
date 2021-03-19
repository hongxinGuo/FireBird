#include"pch.h"
#include"globedef.h"

UINT ThreadUpdateTiingoStockProfileDB(not_null<CWorldMarket*> pMarket) {
  gl_ThreadStatus.IncreaseSavingThread();
  gl_UpdateWorldMarketDB.Wait();
  pMarket->UpdateTiingoStockProfileDB();
  gl_UpdateWorldMarketDB.Signal();
  gl_ThreadStatus.DecreaseSavingThread();

  return 44;
}