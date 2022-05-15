#include"pch.h"
#include"globedef.h"
#include"ThreadStatus.h"
#include"SemaphoreDef.h"
#include"WorldMarket.h"
#include "Thread.h"

UINT ThreadUpdateTiingoCryptoSymbolDB(not_null<CWorldMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();
	gl_UpdateWorldMarketDB.Wait();
	pMarket->UpdateTiingoCryptoSymbolDB();
	gl_UpdateWorldMarketDB.Signal();
	gl_ThreadStatus.DecreaseSavingThread();

	return 44;
}