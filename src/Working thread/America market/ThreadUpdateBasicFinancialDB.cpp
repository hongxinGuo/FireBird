#include"pch.h"

UINT ThreadUpdateBasicFinancialDB() {
	static bool sm_fInProcess = false;

	if (sm_fInProcess) {
		gl_systemMessage.PushErrorMessage(_T("UpdateBasicFinancialDB������ʱ���������"));
		return false;
	}
	else {
		sm_fInProcess = true;
	}
	gl_UpdateWorldMarketDB.acquire();
	gl_dataContainerFinnhubStock.UpdateBasicFinancialDB();
	gl_UpdateWorldMarketDB.release();
	sm_fInProcess = false;

	return 55;
}
