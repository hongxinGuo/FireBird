#include"pch.h"
#include"globedef.h"
#include"SemaphoreDef.h"

#include"WorldMarket.h"

UINT ThreadUpdateNaicsIndustry(not_null<CWorldMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();
	gl_UpdateWorldMarketDB.Wait();
	pMarket->UpdateNaicsIndustry();
	gl_UpdateWorldMarketDB.Signal();
	gl_ThreadStatus.DecreaseSavingThread();

	return 47;
}