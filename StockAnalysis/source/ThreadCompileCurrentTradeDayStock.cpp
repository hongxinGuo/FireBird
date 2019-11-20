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
//由于存在周六和周日，故而当前交易日并不一定就是当前日期，而可能时周五
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadCompileCurrentTradeDayStock(LPVOID) {
  ASSERT(gl_ChinaStockMarket.SystemReady()); // 调用本工作线程时必须设置好市场。

  int i;
  long lDay = gl_systemTime.FormatToDay(gl_ChinaStockMarket.m_ttNewestTransactionTime);
  i = gl_ChinaStockMarket.CompileCurrentTradeDayStock();
  char buffer[30];
  sprintf_s(buffer, "%d", i);
  CString str;
  str = _T("今日处理了");
  str += buffer;
  str += _T("个股票");
  gl_systemMessage.PushInformationMessage(str);
  gl_ChinaStockMarket.CalculateOneDayRelativeStrong(lDay);
  if (gl_systemTime.GetTime() > 150000) {   // 如果中国股市闭市了
    gl_ChinaStockMarket.UpdateStockCodeDB();  // 更新代码。
    gl_ChinaStockMarket.UpdateOptionDB();   // 更新状态
    gl_ChinaStockMarket.SetTodayStockCompiledFlag(true);  // 设置今日已处理标识
  }

  return 5;
}