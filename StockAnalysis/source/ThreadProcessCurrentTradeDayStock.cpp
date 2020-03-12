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
UINT ThreadProcessCurrentTradeDayStock(void) {
  ASSERT(gl_pChinaStockMarket->IsSystemReady()); // 调用本工作线程时必须设置好市场。

  char buffer[30];
  CString str;
  int i;
  long lDay = FormatToDay(gl_pChinaStockMarket->GetNewestTransactionTime());
  if (lDay == gl_pChinaStockMarket->GetDay()) {
    i = gl_pChinaStockMarket->ProcessCurrentTradeDayStock(lDay);
    TRACE("处理今日股票\n");
    sprintf_s(buffer, "%d", i);
    str = _T("今日处理了");
    str += buffer;
    str += _T("个股票");
    gl_systemMessage.PushInformationMessage(str);
    gl_pChinaStockMarket->RunningThreadCalculateThisDayRS(lDay);
    if (gl_pChinaStockMarket->GetTime() > 150400) {   // 如果中国股市闭市了
      gl_pChinaStockMarket->SetRelativeStrongEndDay(gl_pChinaStockMarket->GetDay());
      gl_pChinaStockMarket->SetUpdateStockCodeDB(true);  // 更新代码。
      gl_pChinaStockMarket->SetUpdateOptionDB(true);   // 更新状态
      gl_pChinaStockMarket->SetTodayStockProcessed(true);  // 设置今日已处理标识
    }
  }
  return 14;
}