//////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateInsiderTransactionDB(not_null<CWorldMarket*> pMarket) {
	gl_UpdateWorldMarketDB.Wait();
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->UpdateInsiderTransactionDB();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_UpdateWorldMarketDB.Signal();

	return 48;
}