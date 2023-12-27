#include"pch.h"

UINT ThreadUpdateCryptoExchangeDB() {
	gl_UpdateWorldMarketDB.acquire();
	gl_dataContainerFinnhubCryptoExchange.UpdateDB();
	gl_UpdateWorldMarketDB.release();

	return 51;
}
