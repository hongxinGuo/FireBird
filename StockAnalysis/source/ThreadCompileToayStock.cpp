#include"globedef.h"

#include"DayLine.h"
#include"Market.h"

#include"Thread.h"

using namespace std;
#include<memory>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���߳���ϵͳ�����к���15:05�Զ����ѣ�ÿ��ִֻ��һ��
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadCompileTodayStock(LPVOID) {
  ASSERT(gl_ChinaStockMarket.SystemReady()); // ���ñ������߳�ʱ�������ú��г���

  time_t time = 0;
  switch (gl_systemTime.GetDayOfWeek()) {
  case 6: // ������
    time = gl_systemTime.Gett_time() - 24 * 3600; //
    break;
  case 0: //������
    time = gl_systemTime.Gett_time() - 2 * 24 * 3600; //
    break;
  default: // ����
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
  str = _T("���մ�����");
  str += buffer;
  str += _T("����Ʊ");
  gl_systemMessage.PushInformationMessage(str);
  gl_ChinaStockMarket.CalculateOneDayRelativeStrong(lCurrentTradeDay);
  if (gl_systemTime.GetTime() > 150000) {   // ����й����б�����
    gl_ChinaStockMarket.SaveStockCodeDataBase();  // ���´��롣
    gl_ChinaStockMarket.UpdateOptionDataBase();   // ����״̬
    gl_ChinaStockMarket.SetTodayStockCompiledFlag(true);  // ���ý����Ѵ����ʶ
  }

  return 5;
}