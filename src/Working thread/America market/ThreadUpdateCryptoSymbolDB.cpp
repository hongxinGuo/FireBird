#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

UINT ThreadUpdateFinnhubCryptoSymbolDB(not_null<CWorldMarket*> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateFinnhubCryptoSymbolDB();
	gl_UpdateWorldMarketDB.release();

	return 52;
}
