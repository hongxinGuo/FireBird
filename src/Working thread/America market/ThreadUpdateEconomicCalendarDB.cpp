#include"pch.h"

UINT ThreadUpdateEconomicCalendarDB() {
	gl_UpdateWorldMarketDB.acquire();
	gl_dataContainerFinnhubEconomicCalendar.UpdateDB();
	gl_UpdateWorldMarketDB.release();

	return 50;
}
