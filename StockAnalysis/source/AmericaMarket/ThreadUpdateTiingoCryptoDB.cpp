#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"
#include "Thread.h"

UINT ThreadUpdateTiingoCryptoSymbolDB(not_null<CWorldMarket*> pMarket) {
	gl_UpdateWorldMarketDB.Wait();
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->UpdateTiingoCryptoSymbolDB();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_UpdateWorldMarketDB.Signal();

	return 44;
}