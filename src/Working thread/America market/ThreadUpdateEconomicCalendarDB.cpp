#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateEconomicCalendarDB(not_null<CWorldMarketPtr> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateEconomicCalendarDB();
	gl_UpdateWorldMarketDB.release();

	return 50;
}
