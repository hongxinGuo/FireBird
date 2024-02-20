///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ChinaMarket.h"
#include"TimeConvert.h"

UINT ThreadBuildWeekLine(const not_null<CChinaMarketPtr>& pMarket, long lStartDate) {
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

	gl_dataContainerChinaStock.BuildWeekLine(lStartMonday);

	// 清除当前周周线表
	pMarket->DeleteCurrentWeekWeekLine();
	// 生成新的当前周周线
	pMarket->BuildCurrentWeekWeekLineTable();

	gl_UpdateChinaMarketDB.release();

	return 25;
}
