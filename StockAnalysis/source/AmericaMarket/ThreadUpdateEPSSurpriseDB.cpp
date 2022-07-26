#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

#include"WorldStock.h"

UINT ThreadUpdateEPSSurpriseDB(not_null<CWorldStock*> pStock) {
	gl_UpdateWorldMarketDB.Wait();
	gl_ThreadStatus.IncreaseSavingThread();
	pStock->UpdateEPSSurpriseDB();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_UpdateWorldMarketDB.Signal();

	return 41;
}