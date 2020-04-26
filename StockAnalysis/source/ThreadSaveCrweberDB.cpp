#include"globedef.h"
#include"Semaphore.h"

#include "Thread.h"

Semaphore gl_MaintainDB(1); // Crweber数据库只允许同时一个线程操作之。

UINT ThreadSaveCrweberDB(CCrweberIndexMarket* pMarket, CCrweberIndexPtr pCrweberIndex) {
  gl_MaintainDB.Wait();
  gl_ThreadStatus.IncreaseRunningThread();

  pMarket->SaveCrweberIndexData(pCrweberIndex);

  gl_ThreadStatus.DecreaseRunningThread();
  gl_MaintainDB.Signal();

  return 23;
}

UINT ThreadMaintainCrweberDB(CCrweberIndexMarket* pMarket) {
  gl_MaintainDB.Wait();
  gl_ThreadStatus.IncreaseRunningThread();

  pMarket->LoadDatabase();
  pMarket->SaveDatabase();

  gl_ThreadStatus.DecreaseRunningThread();
  gl_MaintainDB.Signal();

  return 23;
}