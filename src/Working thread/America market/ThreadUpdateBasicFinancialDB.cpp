#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"
#include "Thread.h"

UINT ThreadUpdateBasicFinancialDB(not_null<CWorldMarketPtr> pMarket) {
	static bool sm_fInProcess = false;

	if (sm_fInProcess) {
		gl_systemMessage.PushErrorMessage(_T("UpdateBasicFinancialDB任务用时超过五分钟"));
		return false;
	}
	else {
		sm_fInProcess = true;
	}
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateBasicFinancialDB();
	gl_UpdateWorldMarketDB.release();
	sm_fInProcess = false;

	return 55;
}
