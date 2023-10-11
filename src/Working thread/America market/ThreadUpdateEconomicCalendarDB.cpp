#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateEconomicCalendarDB(not_null<CWorldMarket*> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateEconomicCalendarDB();
	gl_UpdateWorldMarketDB.release();

	return 50;
}
