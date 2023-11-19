#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

UINT ThreadUpdateForexSymbolDB(not_null<CWorldMarketPtr> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateForexSymbolDB();
	gl_UpdateWorldMarketDB.release();

	return 39;
}
