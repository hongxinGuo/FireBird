#include"pch.h"
#include"globedef.h"
#include"SemaphoreDef.h"
#include"WorldMarket.h"

UINT ThreadUpdateBasicFinancialDB(not_null<CWorldMarket*> pMarket) {
	static bool sm_fInProcess = false;

	if (sm_fInProcess) {
		gl_systemMessage.PushErrorMessage(_T("UpdateBasicFinancialDB任务用时超过五分钟"));
		return false;
	}
	else {
		sm_fInProcess = true;
	}
	gl_ThreadStatus.IncreaseSavingThread();
	gl_UpdateWorldMarketDB.Wait();
	pMarket->UpdateBasicFinancialDB();
	gl_UpdateWorldMarketDB.Signal();
	gl_ThreadStatus.DecreaseSavingThread();
	sm_fInProcess = false;

	return 37;
}