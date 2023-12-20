#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

UINT ThreadUpdateTiingoStockDB(const not_null<CWorldMarketPtr>& pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateTiingoStockDB();
	gl_UpdateWorldMarketDB.release();

	return 44;
}
