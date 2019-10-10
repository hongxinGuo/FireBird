#include"globedef.h"

#include"DayLine.h"
#include"Market.h"

#include"Thread.h"

using namespace std;
#include<memory>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 此线程由系统在收市后于15:05自动唤醒，每日只执行一次
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadCompileTodayStock(LPVOID) {
  ASSERT(gl_ChinaStockMarket.SystemReady()); // 调用本工作线程时必须设置好市场。

  time_t time = 0;
  switch (gl_systemTime.GetDayOfWeek()) {
  case 6: // 星期六
    time = gl_systemTime.Gett_time() - 24 * 3600; //
    break;
  case 0: //星期日
    time = gl_systemTime.Gett_time() - 2 * 24 * 3600; //
    break;
  default: // 其他
    time = gl_systemTime.Gett_time();
    break;
  }
  tm tm_;
  localtime_s(&tm_, &time);
  long lCurrentTradeDay = 0;
  if (tm_.tm_hour < 9) {
    lCurrentTradeDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday - 1;
  }
  else {
    lCurrentTradeDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
  }

  int i;
  i = gl_ChinaStockMarket.CompileCurrentTradeDayStocks(lCurrentTradeDay);
  char buffer[30];
  sprintf_s(buffer, "%d", i);
  CString str;
  str = _T("今日处理了");
  str += buffer;
  str += _T("个股票");
  gl_systemMessage.PushInformationMessage(str);
  gl_ChinaStockMarket.CalculateOneDayRelativeStrong(lCurrentTradeDay);
  if (gl_systemTime.GetTime() > 150000) {   // 如果中国股市闭市了
    gl_ChinaStockMarket.SaveStockCodeDataBase();  // 更新代码。
    gl_ChinaStockMarket.UpdateOptionDataBase();   // 更新状态
    gl_ChinaStockMarket.SetTodayStockCompiledFlag(true);  // 设置今日已处理标识
  }

  return 5;
}