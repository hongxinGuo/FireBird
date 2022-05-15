#include"pch.h"
#include"globedef.h"
#include"SystemMessage.h"
#include"ThreadStatus.h"

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

	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->SetProcessingTodayStock();

	const long lDate = pMarket->TransferToMarketDate(pMarket->GetNewestTransactionTime());
	if (lDate == pMarket->GetMarketDate()) {
		pMarket->BuildDayLine(lDate);
		// ���㱾���������ǿ��
		pMarket->BuildDayLineRS(lDate);
		// ������������
		pMarket->BuildWeekLineOfCurrentWeek();
		pMarket->BuildWeekLineRS(GetCurrentMonday(lDate));
		if (pMarket->GetMarketTime() > 150400) {   // ����й����б�����
			pMarket->SetRSEndDate(gl_pChinaMarket->GetMarketDate());
			pMarket->SetUpdateOptionDB(true);   // ����״̬
			pMarket->SetTodayStockProcessed(true);  // ���ý����Ѵ����ʶ
		}
		else {
			pMarket->SetTodayStockProcessed(false);
		}
	}
	gl_ThreadStatus.DecreaseSavingThread();

	return 14;
}