#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateEconomicCalendarDB() {
	gl_UpdateWorldMarketDB.acquire();
	gl_dataFinnhubEconomicCalendar.UpdateDB();
	gl_UpdateWorldMarketDB.release();

	return 50;
}
