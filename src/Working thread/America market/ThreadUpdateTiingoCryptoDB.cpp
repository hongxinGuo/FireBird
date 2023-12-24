#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"
#include "Thread.h"

UINT ThreadUpdateTiingoCryptoSymbolDB() {
	gl_UpdateWorldMarketDB.acquire();
	gl_dataTiingoCryptoSymbol.UpdateDB();
	gl_UpdateWorldMarketDB.release();

	return 54;
}
