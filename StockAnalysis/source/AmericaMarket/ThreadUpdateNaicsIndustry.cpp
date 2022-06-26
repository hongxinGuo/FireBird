#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateNaicsIndustry(not_null<CWorldMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();
	gl_UpdateWorldMarketDB.Wait();
	pMarket->UpdateNaicsIndustry();
	gl_UpdateWorldMarketDB.Signal();
	gl_ThreadStatus.DecreaseSavingThread();

	return 47;
}