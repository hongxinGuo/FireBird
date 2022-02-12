#include"pch.h"
#include"globedef.h"
#include"WorldMarket.h"
#include"SemaphoreDef.h"

UINT ThreadUpdateForexSymbolDB(not_null<CWorldMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();
	gl_UpdateWorldMarketDB.Wait();
	pMarket->UpdateForexSymbolDB();
	gl_UpdateWorldMarketDB.Signal();
	gl_ThreadStatus.DecreaseSavingThread();

	return 39;
}