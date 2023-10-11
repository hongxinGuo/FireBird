#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

UINT ThreadUpdateCryptoExchangeDB(not_null<CWorldMarket*> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateCryptoExchangeDB();
	gl_UpdateWorldMarketDB.release();

	return 51;
}
