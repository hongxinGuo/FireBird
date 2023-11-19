#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

UINT ThreadUpdateFinnhubCryptoSymbolDB(not_null<CWorldMarketPtr> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateFinnhubCryptoSymbolDB();
	gl_UpdateWorldMarketDB.release();

	return 52;
}
