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
UINT ThreadProcessTodayStock(not_null<CChinaMarket*> pMarket) {
  ASSERT(pMarket->IsSystemReady()); // ���ñ������߳�ʱ�������ú��г���

  gl_ThreadStatus.IncreaseRunningThread();
  pMarket->SetProcessingTodayStock();

  const long lDate = FormatToDate(pMarket->GetNewestTransactionTime());
  if (lDate == pMarket->GetFormatedMarketDate()) {
    pMarket->BuildDayLineOfDate(lDate);
    // ���㱾���������ǿ��
    pMarket->BuildDayLineRSOfDate(lDate);
    // ������������
    pMarket->BuildWeekLineOfCurrentWeek();
    pMarket->BuildWeekLineRSOfDate(GetCurrentMonday(lDate));
    if (pMarket->GetFormatedMarketTime() > 150400) {   // ����й����б�����
      pMarket->SetRSEndDate(gl_pChinaStakeMarket->GetFormatedMarketDate());
      pMarket->SetUpdateStakeCodeDB(true);  // ���´��롣
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