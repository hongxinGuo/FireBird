#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

#include"WorldStock.h"

UINT ThreadUpdateEPSSurpriseDB(not_null<CWorldStock*> pStockInput) {
	const auto pStock = pStockInput;
	gl_UpdateWorldMarketDB.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	pStock->UpdateEPSSurpriseDB();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_UpdateWorldMarketDB.release();

	return 41;
}
