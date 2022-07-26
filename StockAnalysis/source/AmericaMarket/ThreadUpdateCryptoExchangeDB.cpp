#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateCryptoExchangeDB(not_null<CWorldMarket*> pMarket) {
	gl_UpdateWorldMarketDB.Wait();
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->UpdateCryptoExchangeDB();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_UpdateWorldMarketDB.Signal();

	return 51;
}