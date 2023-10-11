#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"
#include "Thread.h"

UINT ThreadUpdateTiingoCryptoSymbolDB(not_null<CWorldMarket*> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateTiingoCryptoSymbolDB();
	gl_UpdateWorldMarketDB.release();

	return 44;
}
