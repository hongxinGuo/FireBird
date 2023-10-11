#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

#include"WorldStock.h"

UINT ThreadUpdateEPSSurpriseDB(not_null<CWorldStock*> pStockInput) {
	const auto pStock = pStockInput;
	gl_UpdateWorldMarketDB.acquire();
	pStock->UpdateEPSSurpriseDB();
	gl_UpdateWorldMarketDB.release();

	return 41;
}
