#include"globedef.h"

UINT ThreadUpdateForexSymbolDB(not_null<CAmericaMarket*> pMarket) {
  gl_SaveForexDayLine.Wait();
  pMarket->UpdateForexSymbolDB();
  gl_SaveForexDayLine.Signal();

  return 39;
}