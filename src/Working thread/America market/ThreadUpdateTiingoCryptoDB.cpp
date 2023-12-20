#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"
#include "Thread.h"

UINT ThreadUpdateTiingoCryptoSymbolDB(const not_null<CWorldMarketPtr>& pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateTiingoCryptoSymbolDB();
	gl_UpdateWorldMarketDB.release();

	return 44;
}
