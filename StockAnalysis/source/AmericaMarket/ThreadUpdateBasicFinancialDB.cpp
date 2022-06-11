#include"pch.h"
#include"globedef.h"
#include"ThreadStatus.h"
#include"SystemMessage.h"
#include"SemaphoreDef.h"
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

UINT ThreadUpdateBasicFinancialAnnualDB(not_null<CWorldStock*> pStock) {
	gl_SaveThreadPermitted.Wait();
	gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // 正在工作的线程数加一
	if (!gl_fExitingSystem) {
		pStock->AppendBasicFinancialAnnual();
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // 正在工作的线程数减一
	gl_SaveThreadPermitted.Signal();

	return 56;
}

UINT ThreadUpdateBasicFinancialQuarterlyDB(not_null<CWorldStock*> pStock) {
	gl_SaveThreadPermitted.Wait();
	gl_ThreadStatus.IncreaseBackGroundWorkingthreads();     // 正在工作的线程数加一
	if (!gl_fExitingSystem) {
		pStock->AppendBasicFinancialQuarter();
	}
	gl_ThreadStatus.DecreaseBackGroundWorkingthreads(); // 正在工作的线程数减一
	gl_SaveThreadPermitted.Signal();

	return 57;
}