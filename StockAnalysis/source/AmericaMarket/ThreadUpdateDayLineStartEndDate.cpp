#include"thread.h"

UINT ThreadUpdateWorldStockDayLineStartEndDate(CWorldMarket* pMarket) {
  pMarket->UpdateDayLineStartEndDate();

  return 43;
}