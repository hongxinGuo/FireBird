#include"pch.h"

#include"ThreadStatus.h"
#include"SemaphoreDef.h"
#include"WorldMarket.h"

UINT ThreadUpdateEconomicCalendarDB(not_null<CWorldMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();
	gl_UpdateWorldMarketDB.Wait();
	pMarket->UpdateEconomicCalendarDB();
	gl_UpdateWorldMarketDB.Signal();
	gl_ThreadStatus.DecreaseSavingThread();

	return 50;
}