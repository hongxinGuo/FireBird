#include"pch.h"

UINT ThreadUpdateTiingoStockDB() {
	gl_UpdateWorldMarketDB.acquire();
	gl_dataContainerTiingoStock.UpdateDB();
	gl_UpdateWorldMarketDB.release();

	return 44;
}
