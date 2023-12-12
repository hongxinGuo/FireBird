#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

#include"WorldStock.h"

UINT ThreadUpdateEPSSurpriseDB(not_null<CWorldStockPtr> pStockInput) {
	const auto pStock = pStockInput;
	gl_UpdateWorldMarketDB.acquire();
	pStock->UpdateEPSSurpriseDB();
	gl_UpdateWorldMarketDB.release();

	return 41;
}

UINT ThreadUpdateEPSSurpriseDB2(const CWorldMarketPtr& pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateEPSSurpriseDB();
	gl_UpdateWorldMarketDB.release();

	return 41;
}
