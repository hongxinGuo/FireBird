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
//���ڴ������������գ��ʶ���ǰ�����ղ���һ�����ǵ�ǰ���ڣ�������ʱ����
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadCompileCurrentTradeDayStock(LPVOID) {
  ASSERT(gl_ChinaStockMarket.SystemReady()); // ���ñ������߳�ʱ�������ú��г���

  int i;
  long lDay = gl_systemTime.FormatToDay(gl_ChinaStockMarket.m_ttNewestTransactionTime);
  i = gl_ChinaStockMarket.CompileCurrentTradeDayStock();
  char buffer[30];
  sprintf_s(buffer, "%d", i);
  CString str;
  str = _T("���մ�����");
  str += buffer;
  str += _T("����Ʊ");
  gl_systemMessage.PushInformationMessage(str);
  gl_ChinaStockMarket.CalculateOneDayRelativeStrong(lDay);
  if (gl_systemTime.GetTime() > 150000) {   // ����й����б�����
    gl_ChinaStockMarket.UpdateStockCodeDB();  // ���´��롣
    gl_ChinaStockMarket.UpdateOptionDB();   // ����״̬
    gl_ChinaStockMarket.SetTodayStockCompiledFlag(true);  // ���ý����Ѵ����ʶ
  }

  return 5;
}