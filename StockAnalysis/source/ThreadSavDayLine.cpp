#include"Market.h"

UINT ThreadSaveDayLine(LPVOID) {
  CSingleLock singleLock(&gl_SaveHistoryDayLineData);

  singleLock.Lock();
  gl_ThreadStatus.SetSavingDayLine(true);
  gl_ChinaStockMarket.SaveDayLineData();
  gl_ThreadStatus.SetSavingDayLine(false);
  singleLock.Unlock();

  return 6;
}