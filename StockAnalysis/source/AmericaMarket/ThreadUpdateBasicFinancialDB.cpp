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
	gl_ThreadStatus.IncreaseSavingThread();
	gl_UpdateWorldMarketDB.Wait();
	pMarket->UpdateBasicFinancialDB();
	gl_UpdateWorldMarketDB.Signal();
	gl_ThreadStatus.DecreaseSavingThread();
	sm_fInProcess = false;

	return 55;
}

UINT ThreadUpdateBasicFinancialAnnualDB(vector<CWorldStockPtr> vStock) {
	gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // 正在工作的线程数加一
	for (auto& pStock : vStock) {
		if (!gl_systemStatus.IsExitingSystem()) {
			pStock->AppendBasicFinancialAnnual();
		}
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // 正在工作的线程数减一

	return 56;
}

UINT ThreadUpdateBasicFinancialQuarterlyDB(vector<CWorldStockPtr> vStock) {
	gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // 正在工作的线程数加一
	for (auto& pStock : vStock) {
		if (!gl_systemStatus.IsExitingSystem()) {
			pStock->AppendBasicFinancialQuarter();
		}
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // 正在工作的线程数减一

	return 57;
}