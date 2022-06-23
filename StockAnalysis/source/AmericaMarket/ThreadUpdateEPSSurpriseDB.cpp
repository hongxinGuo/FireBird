#include"pch.h"

#include"ThreadStatus.h"

#include"SemaphoreDef.h"
#include"WorldStock.h"

UINT ThreadUpdateEPSSurpriseDB(not_null<CWorldStock*> pStock) {
	gl_ThreadStatus.IncreaseSavingThread();
	gl_UpdateWorldMarketDB.Wait();
	pStock->UpdateEPSSurpriseDB();
	gl_UpdateWorldMarketDB.Signal();
	gl_ThreadStatus.DecreaseSavingThread();

	return 41;
}