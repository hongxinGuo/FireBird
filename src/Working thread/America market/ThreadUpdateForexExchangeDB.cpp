#include"pch.h"

UINT ThreadUpdateForexExchangeDB() {
	gl_UpdateWorldMarketDB.acquire();
	gl_dataContainerFinnhubForexExchange.UpdateDB();
	gl_UpdateWorldMarketDB.release();

	return 49;
}
