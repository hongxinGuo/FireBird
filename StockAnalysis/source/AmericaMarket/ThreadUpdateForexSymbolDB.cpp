#include"globedef.h"

UINT ThreadUpdateForexSymbolDB(not_null<CAmericaStakeMarket*> pMarket) {
  gl_SaveForexDayLine.Wait();
  pMarket->UpdateForexSymbolDB();
  gl_SaveForexDayLine.Signal();

  return 39;
}