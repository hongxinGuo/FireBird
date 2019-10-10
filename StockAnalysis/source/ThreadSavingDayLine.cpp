#include"Market.h"

UINT ThreadSavingDayLineProc(LPVOID) {
  gl_ThreadStatus.SetSavingDayLine(true);
  gl_ChinaStockMarket.SaveDayLineData();
  gl_ThreadStatus.SetSavingDayLine(false);

  return 6;
}