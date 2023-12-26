#include"pch.h"

UINT ThreadUpdateCryptoExchangeDB() {
	gl_UpdateWorldMarketDB.acquire();
	gl_dataFinnhubCryptoExchange.UpdateDB();
	gl_UpdateWorldMarketDB.release();

	return 51;
}
