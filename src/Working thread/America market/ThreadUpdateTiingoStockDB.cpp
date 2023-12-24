#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

UINT ThreadUpdateTiingoStockDB() {
	gl_UpdateWorldMarketDB.acquire();
	gl_dataTiingoStock.UpdateDB();
	gl_UpdateWorldMarketDB.release();

	return 44;
}
