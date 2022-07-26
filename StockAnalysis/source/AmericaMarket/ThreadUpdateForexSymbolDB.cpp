#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

UINT ThreadUpdateForexSymbolDB(not_null<CWorldMarket*> pMarket) {
	gl_UpdateWorldMarketDB.Wait();
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->UpdateForexSymbolDB();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_UpdateWorldMarketDB.Signal();

	return 39;
}