#include"pch.h"

UINT ThreadUpdateForexSymbolDB() {
	gl_UpdateWorldMarketDB.acquire();
	gl_dataFinnhubForexSymbol.UpdateDB();
	gl_UpdateWorldMarketDB.release();

	return 39;
}
