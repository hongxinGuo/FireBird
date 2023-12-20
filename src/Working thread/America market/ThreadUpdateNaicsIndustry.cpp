#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateNaicsIndustry(const not_null<CWorldMarketPtr>& pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateNaicsIndustry();
	gl_UpdateWorldMarketDB.release();

	return 47;
}
