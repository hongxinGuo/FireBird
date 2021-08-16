#include"pch.h"
#include"globedef.h"

UINT ThreadUpdateCryptoSymbolDB(not_null<CWorldMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();
	gl_UpdateWorldMarketDB.Wait();
	pMarket->UpdateCryptoSymbolDB();
	gl_UpdateWorldMarketDB.Signal();
	gl_ThreadStatus.DecreaseSavingThread();

	return 52;
}