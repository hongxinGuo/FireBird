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

UINT ThreadUpdateBasicFinancialAnnualDB(not_null<CWorldStock*> pStock) {
	gl_SaveBasicFinancialPermitted.Wait();
	gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // ���ڹ������߳�����һ
	if (!gl_systemStatus.IsExitingSystem()) {
		pStock->AppendBasicFinancialAnnual();
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // ���ڹ������߳�����һ
	gl_SaveBasicFinancialPermitted.Signal();

	return 56;
}

UINT ThreadUpdateBasicFinancialQuarterlyDB(not_null<CWorldStock*> pStock) {
	gl_SaveBasicFinancialPermitted.Wait();
	gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // ���ڹ������߳�����һ
	if (!gl_systemStatus.IsExitingSystem()) {
		pStock->AppendBasicFinancialQuarter();
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // ���ڹ������߳�����һ
	gl_SaveBasicFinancialPermitted.Signal();

	return 57;
}