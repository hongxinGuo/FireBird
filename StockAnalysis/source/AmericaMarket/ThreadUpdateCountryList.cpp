#include"globedef.h"

UINT ThreadUpdateCountryListDB(not_null<CWorldMarket*> pMarket) {
  pMarket->UpdateCountryListDB();

  return 40;
}