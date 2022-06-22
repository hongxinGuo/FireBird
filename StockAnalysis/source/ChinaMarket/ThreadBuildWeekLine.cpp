///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"globedef.h"

#include"ThreadStatus.h"
#include"Thread.h"
#include"SemaphoreDef.h"

#include"ChinaMarket.h"

UINT ThreadBuildWeekLine(not_null<CChinaMarket*> pMarket, long lStartDate) {
	gl_ThreadStatus.IncreaseSavingThread();
	gl_ThreadStatus.SetCreatingWeekLine(true);

	long lStartMonday = GetCurrentMonday(lStartDate);
	long year = lStartMonday / 10000;
	long month = lStartMonday / 100 - (lStartMonday / 10000) * 100;
	long mday = lStartMonday - (lStartMonday / 100) * 100;
	CTime ctCurrent(year, month, mday, 12, 0, 0), ctNext;
	const CTimeSpan ts7Day(7, 0, 0, 0);
	long lCurrentMonday = lStartMonday;

	if (lStartDate > 19900101) { // 目前此种情况只用于重新生成本周周线
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

	// 清除当前周周线表
	pMarket->DeleteCurrentWeekWeekLine();
	// 生成新的当前周周线
	pMarket->BuildCurrentWeekWeekLineTable();

	gl_ThreadStatus.SetCreatingWeekLine(false);
	gl_ThreadStatus.DecreaseSavingThread();

	return 25;
}

UINT ThreadBuildWeekLineOfStock(not_null<CChinaStock*> pStock, long lStartDate) {
	gl_ThreadStatus.IncreaseSavingThread();
	gl_SemaphoreBackGroundTaskThreads.Wait();
	gl_ThreadStatus.IncreaseBackGroundWorkingthreads();
	if (!gl_systemStatus.IsExitingSystem()) pStock->BuildWeekLine(lStartDate);

	gl_ThreadStatus.DecreaseBackGroundWorkingthreads();
	gl_SemaphoreBackGroundTaskThreads.Signal();
	gl_ThreadStatus.DecreaseSavingThread();

	return 26;
}

UINT ThreadBuildWeekLineOfCurrentWeek(not_null<CChinaMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();
	gl_SemaphoreBackGroundTaskThreads.Wait();
	gl_ThreadStatus.IncreaseBackGroundWorkingthreads();

	pMarket->BuildWeekLineOfCurrentWeek();

	gl_ThreadStatus.DecreaseBackGroundWorkingthreads();
	gl_SemaphoreBackGroundTaskThreads.Signal();
	gl_ThreadStatus.DecreaseSavingThread();

	return 32;
}

UINT ThreadBuildCurrentWeekWeekLineTable(not_null<CChinaMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();

	// 清除当前周周线表
	pMarket->DeleteCurrentWeekWeekLine();

	// 生成新的当前周周线
	pMarket->BuildCurrentWeekWeekLineTable();

	gl_ThreadStatus.DecreaseSavingThread();

	return 33;
}