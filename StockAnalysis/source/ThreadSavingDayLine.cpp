#include"Market.h"

UINT ThreadSavingDayLineProc(LPVOID) {
  gl_ThreadStatus.SetSavingDayLineInProcess(true);
  gl_ChinaStockMarket.SaveDayLineData();
  gl_ThreadStatus.SetSavingDayLineInProcess(false);

  return 6;
}