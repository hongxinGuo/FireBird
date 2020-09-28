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

  long lDay = FormatToDay(pMarket->GetNewestTransactionTime());
  if (lDay == pMarket->GetFormatedMarketDate()) {
    pMarket->BuildDayLineOfDay(lDay);
    // 计算本日日线相对强度
    pMarket->BuildDayLineRSOfDay(lDay);
    // 生成周线数据
    pMarket->BuildWeekLineOfCurrentWeek();
    pMarket->BuildWeekLineRSOfDay(GetCurrentMonday(lDay));
    if (pMarket->GetFormatedMarketTime() > 150400) {   // 如果中国股市闭市了
      pMarket->SetRelativeStrongEndDay(gl_pChinaStockMarket->GetFormatedMarketDate());
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