#include"globedef.h"

UINT ThreadUpdateCountryListDB(not_null<CAmericaMarket*> pMarket) {
  pMarket->UpdateCountryListDB();

  return 40;
}