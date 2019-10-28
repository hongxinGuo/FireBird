#include"Market.h"

UINT ThreadSaveDayLine(LPVOID) {
  static int counter = 0;
  CSingleLock singleLock(&gl_SaveHistoryDayLineData);

  singleLock.Lock();
  if (singleLock.IsLocked()) {
    counter++;
    if (counter > 1) {
      int a = 0;
    }
    gl_ChinaStockMarket.SaveDayLineData();
    counter--;
    singleLock.Unlock();
  }

  return 6;
}