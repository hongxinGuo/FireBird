#include"pch.h"

UINT ThreadUpdateEconomicCalendarDB() {
	gl_UpdateWorldMarketDB.acquire();
	gl_dataFinnhubEconomicCalendar.UpdateDB();
	gl_UpdateWorldMarketDB.release();

	return 50;
}
