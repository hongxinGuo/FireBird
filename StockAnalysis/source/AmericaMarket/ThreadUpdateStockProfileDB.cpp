#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateWorldMarketStockProfileDB(not_null<CWorldMarket*> pMarket) {
	static bool sm_fInProcess = false;

	if (sm_fInProcess) {
		gl_systemMessage.PushErrorMessage(_T("UpdateWorldMarektStockProfileDB任务用时超过五分钟"));
		return false;
	}
	else {
		sm_fInProcess = true;
	}
	gl_UpdateWorldMarketDB.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->UpdateStockProfileDB();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_UpdateWorldMarketDB.release();
	sm_fInProcess = false;

	return 37;
}