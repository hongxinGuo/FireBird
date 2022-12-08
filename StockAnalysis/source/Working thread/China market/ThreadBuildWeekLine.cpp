///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ThreadStatus.h"
#include"Thread.h"
#include"TimeConvert.h"

#include"ChinaMarket.h"

UINT ThreadBuildWeekLine(not_null<CChinaMarket*> pMarket, long lStartDate) {
	gl_ThreadStatus.IncreaseSavingThread();

	long lStartMonday = GetCurrentMonday(lStartDate);
	long year = lStartMonday / 10000;
	long month = lStartMonday / 100 - (lStartMonday / 10000) * 100;
	long mday = lStartMonday - (lStartMonday / 100) * 100;
	CTime ctCurrent(year, month, mday, 12, 0, 0), ctNext;
	const CTimeSpan ts7Day(7, 0, 0, 0);
	long lCurrentMonday = lStartMonday;

	if (lStartDate > 19900101) { // Ŀǰ�������ֻ�����������ɱ�������
		ASSERT(lStartMonday == GetCurrentMonday(pMarket->GetMarketDate()));
		do {
			pMarket->DeleteWeekLine(lCurrentMonday);
			ctCurrent += ts7Day;
			lCurrentMonday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
		} while (lCurrentMonday <= pMarket->GetMarketDate());
	}
	else {
		pMarket->DeleteWeekLine();
	}

	pMarket->BuildWeekLine(lStartMonday);

	// �����ǰ�����߱�
	pMarket->DeleteCurrentWeekWeekLine();
	// �����µĵ�ǰ������
	pMarket->BuildCurrentWeekWeekLineTable();

	gl_ThreadStatus.DecreaseSavingThread();

	return 25;
}

UINT ThreadBuildWeekLineOfStock(not_null<CChinaStock*> pStock, long lStartDate) {
	gl_BackGroundTaskThread.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	gl_ThreadStatus.IncreaseBackGroundWorkingThread();
	if (!gl_systemStatus.IsExitingSystem()) pStock->BuildWeekLine(lStartDate);

	gl_ThreadStatus.DecreaseBackGroundWorkingThread();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_BackGroundTaskThread.release();

	return 26;
}

UINT ThreadBuildWeekLineOfCurrentWeek(not_null<CChinaMarket*> pMarket) {
	gl_BackGroundTaskThread.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	gl_ThreadStatus.IncreaseBackGroundWorkingThread();

	pMarket->BuildWeekLineOfCurrentWeek();

	gl_ThreadStatus.DecreaseBackGroundWorkingThread();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_BackGroundTaskThread.release();

	return 32;
}

UINT ThreadBuildCurrentWeekWeekLineTable(not_null<CChinaMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();

	// �����ǰ�����߱�
	pMarket->DeleteCurrentWeekWeekLine();

	// �����µĵ�ǰ������
	pMarket->BuildCurrentWeekWeekLineTable();

	gl_ThreadStatus.DecreaseSavingThread();

	return 33;
}