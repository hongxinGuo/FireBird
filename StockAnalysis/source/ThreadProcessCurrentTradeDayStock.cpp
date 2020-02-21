#include"globedef.h"

#include"DayLine.h"
#include"ChinaMarket.h"

#include"Thread.h"

using namespace std;
#include<memory>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���߳���ϵͳ�����к���15:05�Զ����ѣ�ÿ��ִֻ��һ��
//
//
//���ڴ������������գ��ʶ���ǰ�����ղ���һ�����ǵ�ǰ���ڣ�������������
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadProcessCurrentTradeDayStock(LPVOID) {
  ASSERT(gl_ChinaStockMarket.IsSystemReady()); // ���ñ������߳�ʱ�������ú��г���

  int i;
  long lDay = FormatToDay(gl_ChinaStockMarket.GetNewestTransactionTime());
  i = gl_ChinaStockMarket.ProcessCurrentTradeDayStock(lDay);
  TRACE("������չ�Ʊ\n");
  char buffer[30];
  sprintf_s(buffer, "%d", i);
  CString str;
  str = _T("���մ�����");
  str += buffer;
  str += _T("����Ʊ");
  gl_systemMessage.PushInformationMessage(str);
  AfxBeginThread(ThreadCalculateThisDayRS, (LPVOID)lDay, THREAD_PRIORITY_LOWEST);
  if (gl_ChinaStockMarket.GetTime() > 150400) {   // ����й����б�����
    gl_ChinaStockMarket.SetRelativeStrongEndDay(gl_ChinaStockMarket.GetDay());
    gl_ChinaStockMarket.SetUpdateStockCodeDB(true);  // ���´��롣
    gl_ChinaStockMarket.SetUpdateOptionDB(true);   // ����״̬
    gl_ChinaStockMarket.SetTodayStockProcessed(true);  // ���ý����Ѵ����ʶ
  }

  return 14;
}