///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ThreadStatus.h"
#include"Thread.h"
#include"TimeConvert.h"

#include"ChinaMarket.h"

UINT ThreadBuildWeekLine(not_null<CChinaMarketPtr> pMarket, long lStartDate) {
	gl_UpdateChinaMarketDB.acquire();

	const long lStartMonday = GetCurrentMonday(lStartDate);
	const long year = lStartMonday / 10000;
	const long month = lStartMonday / 100 - (lStartMonday / 10000) * 100;
	const long mDay = lStartMonday - (lStartMonday / 100) * 100;
	CTime ctCurrent(year, month, mDay, 12, 0, 0);
	const CTimeSpan ts7Day(7, 0, 0, 0);
	long lCurrentMonday = lStartMonday;

	if (lStartDate > 19900101) {// 目前此种情况只用于重新生成本周周线
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

	gl_UpdateChinaMarketDB.release();

	return 25;
}

UINT ThreadBuildWeekLineOfStock(not_null<CChinaStockPtr> pStockInput, long lStartDate) {
	const auto pStock = pStockInput;
	gl_UpdateChinaMarketDB.acquire();
	if (!gl_systemConfiguration.IsExitingSystem()) pStock->BuildWeekLine(lStartDate);
	gl_UpdateChinaMarketDB.release();

	return 26;
}

UINT ThreadBuildWeekLineOfCurrentWeek(not_null<CChinaMarketPtr> pMarket) {
	gl_UpdateChinaMarketDB.acquire();
	pMarket->BuildWeekLineOfCurrentWeek();
	gl_UpdateChinaMarketDB.release();

	return 32;
}

UINT ThreadBuildCurrentWeekWeekLineTable(not_null<CChinaMarketPtr> pMarket) {
	gl_UpdateChinaMarketDB.acquire();
	// 清除当前周周线表
	pMarket->DeleteCurrentWeekWeekLine();

	// 生成新的当前周周线
	pMarket->BuildCurrentWeekWeekLineTable();

	gl_UpdateChinaMarketDB.release();

	return 33;
}
