#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"
#include "Thread.h"

UINT ThreadUpdateBasicFinancialDB(not_null<CWorldMarket*> pMarket) {
	static bool sm_fInProcess = false;

	if (sm_fInProcess) {
		gl_systemMessage.PushErrorMessage(_T("UpdateBasicFinancialDB任务用时超过五分钟"));
		return false;
	}
	else {
		sm_fInProcess = true;
	}
	gl_UpdateWorldMarketDB.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->UpdateBasicFinancialDB();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_UpdateWorldMarketDB.release();
	sm_fInProcess = false;

	return 55;
}

UINT ThreadUpdateBasicFinancialAnnualDB(vector<CWorldStockPtr> vStock) {
	gl_ThreadStatus.IncreaseBackGroundWorkingThread(); // 正在工作的线程数加一
	for (const auto& pStock : vStock) {
		if (gl_systemStatus.IsExitingSystem()) break;
		pStock->AppendBasicFinancialAnnual();
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingThread(); // 正在工作的线程数减一

	return 56;
}

UINT ThreadUpdateBasicFinancialQuarterlyDB(vector<CWorldStockPtr> vStock) {
	gl_ThreadStatus.IncreaseBackGroundWorkingThread(); // 正在工作的线程数加一
	for (const auto& pStock : vStock) {
		if (gl_systemStatus.IsExitingSystem()) break;
		pStock->AppendBasicFinancialQuarter();
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingThread(); // 正在工作的线程数减一

	return 57;
}
