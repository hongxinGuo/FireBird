#include"globedef.h"

#include"DayLine.h"
#include"SetDayLineInfo.h"
#include"Market.h"

#include"Thread.h"

using namespace std;
#include<memory>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 日线装载工作线程
//
// 从数据库中装入相应股票的日线数据，然后计算各相对强度
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadLoadDayLine(LPVOID) {
  ASSERT(gl_ChinaStockMarket.m_pCurrentStock != nullptr);
  ASSERT(!gl_ChinaStockMarket.m_pCurrentStock->IsDayLineLoaded());

  // 装入日线数据
  gl_ChinaStockMarket.m_pCurrentStock->LoadDayLine();

  // 计算各相对强度
  gl_ChinaStockMarket.m_pCurrentStock->CalculateDayLineRS();

  gl_ChinaStockMarket.m_pCurrentStock->SetDayLineLoaded(true);

  return 7;
}
