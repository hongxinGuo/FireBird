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
UINT ThreadProcessCurrentTradeDayStock(void) {
  ASSERT(gl_pChinaStockMarket->IsSystemReady()); // ���ñ������߳�ʱ�������ú��г���

  char buffer[30];
  CString str;
  int i;
  long lDay = FormatToDay(gl_pChinaStockMarket->GetNewestTransactionTime());
  if (lDay == gl_pChinaStockMarket->GetDay()) {
    i = gl_pChinaStockMarket->ProcessCurrentTradeDayStock(lDay);
    TRACE("������չ�Ʊ\n");
    sprintf_s(buffer, "%d", i);
    str = _T("���մ�����");
    str += buffer;
    str += _T("����Ʊ");
    gl_systemMessage.PushInformationMessage(str);
    gl_pChinaStockMarket->RunningThreadCalculateThisDayRS(lDay);
    if (gl_pChinaStockMarket->GetTime() > 150400) {   // ����й����б�����
      gl_pChinaStockMarket->SetRelativeStrongEndDay(gl_pChinaStockMarket->GetDay());
      gl_pChinaStockMarket->SetUpdateStockCodeDB(true);  // ���´��롣
      gl_pChinaStockMarket->SetUpdateOptionDB(true);   // ����״̬
      gl_pChinaStockMarket->SetTodayStockProcessed(true);  // ���ý����Ѵ����ʶ
    }
  }
  return 14;
}