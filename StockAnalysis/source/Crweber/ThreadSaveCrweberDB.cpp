#include"pch.h"
#include"globedef.h"
#include"SemaphoreDef.h"

#include "Thread.h"

UINT ThreadSaveCrweberDB(not_null<CCrweberIndexMarket*> pMarket, not_null<CCrweberIndexPtr> pCrweberIndex) {
	gl_MaintainCrweberDB.Wait();
	gl_ThreadStatus.IncreaseSavingThread();

	pMarket->SaveCrweberIndexData(pCrweberIndex);

	gl_ThreadStatus.DecreaseSavingThread();
	gl_MaintainCrweberDB.Signal();

	return 23;
}

UINT ThreadMaintainCrweberDB(not_null<CCrweberIndexMarket*> pMarket) {
	gl_MaintainCrweberDB.Wait();
	gl_ThreadStatus.IncreaseSavingThread();

	pMarket->LoadDatabase();
	pMarket->SaveDatabase();

	gl_ThreadStatus.DecreaseSavingThread();
	gl_MaintainCrweberDB.Signal();

	return 24;
}