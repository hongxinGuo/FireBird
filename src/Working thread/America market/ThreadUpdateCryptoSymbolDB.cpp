#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

UINT ThreadUpdateFinnhubCryptoSymbolDB() {
	gl_UpdateWorldMarketDB.acquire();
	gl_dataFinnhubCryptoSymbol.UpdateDB();
	gl_UpdateWorldMarketDB.release();

	return 52;
}
