#include"pch.h"
#include"globedef.h"
#include "..\Thread.h"

UINT ThreadUpdateTiingoCryptoDB(not_null<CWorldMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();
	gl_UpdateWorldMarketDB.Wait();
	pMarket->UpdateTiingoCryptoDB();
	gl_UpdateWorldMarketDB.Signal();
	gl_ThreadStatus.DecreaseSavingThread();

	return 44;
}