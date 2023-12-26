#include"pch.h"

UINT ThreadUpdateTiingoCryptoSymbolDB() {
	gl_UpdateWorldMarketDB.acquire();
	gl_dataTiingoCryptoSymbol.UpdateDB();
	gl_UpdateWorldMarketDB.release();

	return 54;
}
