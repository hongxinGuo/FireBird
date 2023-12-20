#include"pch.h"

#include"ThreadStatus.h"

#include"thread.h"
#include"WorldMarket.h"

UINT ThreadUpdateWorldStockDayLineStartEndDate(const CWorldMarketPtr& pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateStockDayLineStartEndDate();
	gl_UpdateWorldMarketDB.release();

	return 43;
}
