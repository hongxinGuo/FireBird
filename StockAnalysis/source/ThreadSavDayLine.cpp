#include"Market.h"

UINT ThreadSaveDayLine(LPVOID) {
  gl_ThreadStatus.SetSavingDayLine(true);
  gl_ChinaStockMarket.SaveDayLineData();
  gl_ThreadStatus.SetSavingDayLine(false);

  return 6;
}