#include"pch.h"
#include"globedef.h"
#include"SemaphoreDef.h"

#include "Thread.h"

Semaphore gl_MaintainDB(1); // Crweber数据库只允许同时一个线程操作之。

UINT ThreadSaveCrweberDB(not_null<CCrweberIndexMarket*> pMarket, not_null<CCrweberIndexPtr> pCrweberIndex) {
	gl_MaintainDB.Wait();
	gl_ThreadStatus.IncreaseSavingThread();

	pMarket->SaveCrweberIndexData(pCrweberIndex);

	gl_ThreadStatus.DecreaseSavingThread();
	gl_MaintainDB.Signal();

	return 23;
}

UINT ThreadMaintainCrweberDB(not_null<CCrweberIndexMarket*> pMarket) {
	gl_MaintainDB.Wait();
	gl_ThreadStatus.IncreaseSavingThread();

	pMarket->LoadDatabase();
	pMarket->SaveDatabase();

	gl_ThreadStatus.DecreaseSavingThread();
	gl_MaintainDB.Signal();

	return 24;
}