#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateForexExchangeDB(not_null<CWorldMarket*> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->UpdateForexExchangeDB();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_UpdateWorldMarketDB.release();

	return 49;
}