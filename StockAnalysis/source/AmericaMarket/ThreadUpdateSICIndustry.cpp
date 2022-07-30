#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateSICIndustry(not_null<CWorldMarket*> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->UpdateSICIndustry();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_UpdateWorldMarketDB.release();

	return 46;
}