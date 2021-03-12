#include"pch.h"
#include"globedef.h"

#include"thread.h"

UINT ThreadUpdateWorldStockDayLineStartEndDate(CWorldMarket* pMarket) {
  gl_ThreadStatus.IncreaseSavingThread();
  pMarket->UpdateStockDayLineStartEndDate();
  gl_ThreadStatus.DecreaseSavingThread();

  return 43;
}