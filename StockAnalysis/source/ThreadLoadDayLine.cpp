#include"globedef.h"

#include"ChinaMarket.h"
#include"Thread.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 日线装载工作线程
//
// 从数据库中装入相应股票的日线数据，然后计算各相对强度
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadLoadDayLine(LPVOID) {
  CStockPtr pCurrentStock = gl_ChinaStockMarket.GetCurrentStockPtr();

  ASSERT(pCurrentStock != nullptr);
  ASSERT(!pCurrentStock->IsDayLineLoaded());

  // 装入日线数据
  pCurrentStock->LoadDayLineAndDayLineInfo();
  // 计算各相对强度
  pCurrentStock->CalculateDayLineRS();
  pCurrentStock->SetDayLineLoaded(true);

  return 16;
}