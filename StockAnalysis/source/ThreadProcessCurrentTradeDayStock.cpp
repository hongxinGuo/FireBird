#include"globedef.h"

#include"DayLine.h"
#include"ChinaMarket.h"
#include"Thread.h"

using namespace std;
#include<thread>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 此线程由系统在收市后于15:05自动唤醒，每日只执行一次
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadProcessTodayStock(CChinaMarket* pMarket) {
  ASSERT(pMarket->IsSystemReady()); // 调用本工作线程时必须设置好市场。

  gl_ThreadStatus.IncreaseRunningThread();
  pMarket->SetProcessingTodayStock();

  long lDate = FormatToDate(pMarket->GetNewestTransactionTime());
  if (lDate == pMarket->GetFormatedMarketDate()) {
    pMarket->BuildDayLineOfDay(lDate);
    // 计算本日日线相对强度
    pMarket->BuildDayLineRSOfDay(lDate);
    // 生成周线数据
    pMarket->BuildWeekLineOfCurrentWeek();
    pMarket->BuildWeekLineRSOfDay(GetCurrentMonday(lDate));
    if (pMarket->GetFormatedMarketTime() > 150400) {   // 如果中国股市闭市了
      pMarket->SetRSEndDate(gl_pChinaStockMarket->GetFormatedMarketDate());
      pMarket->SetUpdateStockCodeDB(true);  // 更新代码。
      pMarket->SetUpdateOptionDB(true);   // 更新状态
      pMarket->SetTodayStockProcessed(true);  // 设置今日已处理标识
    }
    else {
      pMarket->SetTodayStockProcessed(false);
    }
  }
  gl_ThreadStatus.DecreaseRunningThread();

  return 14;
}