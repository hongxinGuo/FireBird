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
	gl_UpdateWorldMarketDB.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->UpdateBasicFinancialDB();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_UpdateWorldMarketDB.release();
	sm_fInProcess = false;

	return 55;
}

UINT ThreadUpdateBasicFinancialAnnualDB(vector<CWorldStockPtr> vStock) {
	gl_ThreadStatus.IncreaseBackGroundWorkingThread(); // ���ڹ������߳�����һ
	for (const auto& pStock : vStock) {
		if (gl_systemStatus.IsExitingSystem()) break;
		pStock->AppendBasicFinancialAnnual();
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingThread(); // ���ڹ������߳�����һ

	return 56;
}

UINT ThreadUpdateBasicFinancialQuarterlyDB(vector<CWorldStockPtr> vStock) {
	gl_ThreadStatus.IncreaseBackGroundWorkingThread(); // ���ڹ������߳�����һ
	for (const auto& pStock : vStock) {
		if (gl_systemStatus.IsExitingSystem()) break;
		pStock->AppendBasicFinancialQuarter();
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingThread(); // ���ڹ������߳�����һ

	return 57;
}
