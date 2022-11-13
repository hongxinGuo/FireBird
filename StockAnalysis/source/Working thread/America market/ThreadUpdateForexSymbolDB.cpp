#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

UINT ThreadUpdateForexSymbolDB(not_null<CWorldMarket*> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->UpdateForexSymbolDB();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_UpdateWorldMarketDB.release();

	return 39;
}