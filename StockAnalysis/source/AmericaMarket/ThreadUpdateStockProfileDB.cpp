#include"pch.h"
#include"globedef.h"
#include"WorldMarket.h"

UINT ThreadUpdateStockProfileDB(not_null<CWorldMarket*> pMarket) {
	static bool sm_fInProcess = false;

	if (sm_fInProcess) {
		gl_systemMessage.PushErrorMessage(_T("UpdateStockProfileDB任务用时超过五分钟"));
		return false;
	}
	else {
		sm_fInProcess = true;
	}
	gl_ThreadStatus.IncreaseSavingThread();
	gl_UpdateWorldMarketDB.Wait();
	pMarket->UpdateStockProfileDB();
	gl_UpdateWorldMarketDB.Signal();
	gl_ThreadStatus.DecreaseSavingThread();
	sm_fInProcess = false;

	return 37;
}