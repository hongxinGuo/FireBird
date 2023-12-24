#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateForexExchangeDB() {
	gl_UpdateWorldMarketDB.acquire();
	gl_dataFinnhubForexExchange.UpdateDB();
	gl_UpdateWorldMarketDB.release();

	return 49;
}
