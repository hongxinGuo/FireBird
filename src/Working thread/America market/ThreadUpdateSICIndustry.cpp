#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateSICIndustry(not_null<CWorldMarket*> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateSICIndustry();
	gl_UpdateWorldMarketDB.release();

	return 46;
}
