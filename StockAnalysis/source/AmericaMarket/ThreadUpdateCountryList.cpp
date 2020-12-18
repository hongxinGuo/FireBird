#include"globedef.h"

UINT ThreadUpdateCountryListDB(not_null<CAmericaStakeMarket*> pMarket) {
  pMarket->UpdateCountryListDB();

  return 40;
}