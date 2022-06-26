#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateTiingoStockDB(not_null<CWorldMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();
	gl_UpdateWorldMarketDB.Wait();
	pMarket->UpdateTiingoStockDB();
	gl_UpdateWorldMarketDB.Signal();
	gl_ThreadStatus.DecreaseSavingThread();

	return 44;
}