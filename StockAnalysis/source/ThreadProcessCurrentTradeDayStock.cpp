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
  pMarket->SetProcessingTodayStock();

  long lDay = FormatToDay(pMarket->GetNewestTransactionTime());
  if (lDay == pMarket->GetFormatedMarketDate()) {
    pMarket->BuildDayLineOfDay(lDay);
    // ���㱾���������ǿ��
    pMarket->BuildDayLineRSOfDay(lDay);
    // ������������
    pMarket->BuildWeekLineOfCurrentWeek();
    pMarket->BuildWeekLineRSOfDay(GetCurrentMonday(lDay));
    if (pMarket->GetFormatedMarketTime() > 150400) {   // ����й����б�����
      pMarket->SetRelativeStrongEndDay(gl_pChinaStockMarket->GetFormatedMarketDate());
      pMarket->SetUpdateStockCodeDB(true);  // ���´��롣
      pMarket->SetUpdateOptionDB(true);   // ����״̬
      pMarket->SetTodayStockProcessed(true);  // ���ý����Ѵ����ʶ
    }
    else {
      pMarket->SetTodayStockProcessed(false);
    }
  }
  gl_ThreadStatus.DecreaseRunningThread();

  return 14;
}