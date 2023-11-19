#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

UINT ThreadUpdateTiingoStockDB(not_null<CWorldMarketPtr> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateTiingoStockDB();
	gl_UpdateWorldMarketDB.release();

	return 44;
}
