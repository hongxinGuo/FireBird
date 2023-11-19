#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateTiingoIndustry(not_null<CWorldMarketPtr> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateTiingoIndustry();
	gl_UpdateWorldMarketDB.release();

	return 45;
}
