#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"
#include "Thread.h"

UINT ThreadUpdateBasicFinancialDB(not_null<CWorldMarket*> pMarket) {
	static bool sm_fInProcess = false;

	if (sm_fInProcess) {
		gl_systemMessage.PushErrorMessage(_T("UpdateBasicFinancialDB������ʱ���������"));
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
	gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // ���ڹ������߳�����һ
	for (auto& pStock : vStock) {
		if (!gl_systemStatus.IsExitingSystem()) {
			pStock->AppendBasicFinancialAnnual();
		}
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // ���ڹ������߳�����һ

	return 56;
}

UINT ThreadUpdateBasicFinancialQuarterlyDB(vector<CWorldStockPtr> vStock) {
	gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // ���ڹ������߳�����һ
	for (auto& pStock : vStock) {
		if (!gl_systemStatus.IsExitingSystem()) {
			pStock->AppendBasicFinancialQuarter();
		}
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // ���ڹ������߳�����һ

	return 57;
}