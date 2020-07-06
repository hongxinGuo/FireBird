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
  char buffer[30];
  CString str;
  int i;
  long lDay = FormatToDay(pMarket->GetNewestTransactionTime());
  if (lDay == pMarket->GetFormatedMarketDay()) {
    i = pMarket->ProcessCurrentTradeDayStock(lDay);
    TRACE("处理今日股票\n");
    sprintf_s(buffer, "%d", i);
    str = _T("今日处理了");
    str += buffer;
    str += _T("个股票");
    gl_systemMessage.PushInformationMessage(str);
    pMarket->RunningThreadCalculateThisDayRS(lDay);
    if (pMarket->GetFormatedMarketTime() > 150400) {   // 如果中国股市闭市了
      pMarket->SetRelativeStrongEndDay(gl_pChinaStockMarket->GetFormatedMarketDay());
      pMarket->SetUpdateStockCodeDB(true);  // 更新代码。
      pMarket->SetUpdateOptionDB(true);   // 更新状态
      pMarket->SetTodayStockProcessed(true);  // 设置今日已处理标识
    }
  }
  gl_ThreadStatus.DecreaseRunningThread();

  return 14;
}