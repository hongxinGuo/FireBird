#include"globedef.h"

#include"DayLine.h"
#include"ChinaMarket.h"
#include"Thread.h"

using namespace std;
#include<thread>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���߳���ϵͳ�����к���15:05�Զ����ѣ�ÿ��ִֻ��һ��
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadProcessCurrentTradeDayStock(LPVOID) {
  ASSERT(gl_ChinaStockMarket.IsSystemReady()); // ���ñ������߳�ʱ�������ú��г���

  char buffer[30];
  CString str;
  int i;
  long lDay = FormatToDay(gl_ChinaStockMarket.GetNewestTransactionTime());
  if (lDay == gl_ChinaStockMarket.GetDay()) {
    i = gl_ChinaStockMarket.ProcessCurrentTradeDayStock(lDay);
    TRACE("������չ�Ʊ\n");
    sprintf_s(buffer, "%d", i);
    str = _T("���մ�����");
    str += buffer;
    str += _T("����Ʊ");
    gl_systemMessage.PushInformationMessage(str);
    thread thread_calculateRS(ThreadCalculateThisDayRS, (LPVOID)lDay);
    thread_calculateRS.detach();
    if (gl_ChinaStockMarket.GetTime() > 150400) {   // ����й����б�����
      gl_ChinaStockMarket.SetRelativeStrongEndDay(gl_ChinaStockMarket.GetDay());
      gl_ChinaStockMarket.SetUpdateStockCodeDB(true);  // ���´��롣
      gl_ChinaStockMarket.SetUpdateOptionDB(true);   // ����״̬
      gl_ChinaStockMarket.SetTodayStockProcessed(true);  // ���ý����Ѵ����ʶ
    }
  }
  return 14;
}