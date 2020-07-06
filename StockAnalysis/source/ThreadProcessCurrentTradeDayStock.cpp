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
UINT ThreadProcessTodayStock(CChinaMarket* pMarket) {
  ASSERT(pMarket->IsSystemReady()); // ���ñ������߳�ʱ�������ú��г���

  gl_ThreadStatus.IncreaseRunningThread();
  char buffer[30];
  CString str;
  int i;
  long lDay = FormatToDay(pMarket->GetNewestTransactionTime());
  if (lDay == pMarket->GetFormatedMarketDay()) {
    i = pMarket->ProcessCurrentTradeDayStock(lDay);
    TRACE("������չ�Ʊ\n");
    sprintf_s(buffer, "%d", i);
    str = _T("���մ�����");
    str += buffer;
    str += _T("����Ʊ");
    gl_systemMessage.PushInformationMessage(str);
    pMarket->RunningThreadCalculateThisDayRS(lDay);
    if (pMarket->GetFormatedMarketTime() > 150400) {   // ����й����б�����
      pMarket->SetRelativeStrongEndDay(gl_pChinaStockMarket->GetFormatedMarketDay());
      pMarket->SetUpdateStockCodeDB(true);  // ���´��롣
      pMarket->SetUpdateOptionDB(true);   // ����״̬
      pMarket->SetTodayStockProcessed(true);  // ���ý����Ѵ����ʶ
    }
  }
  gl_ThreadStatus.DecreaseRunningThread();

  return 14;
}