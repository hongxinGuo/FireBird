#include"pch.h"

#include"ThreadStatus.h"

#include"WorldMarket.h"

UINT ThreadUpdateWorldMarketStockProfileDB() {
	static bool sm_fProcessing = false;

	if (sm_fProcessing) {
		gl_systemMessage.PushErrorMessage(_T("UpdateWorldMarektStockProfileDB任务用时超过五分钟"));
		return 37;
	}
	else {
		sm_fProcessing = true;
	}
	gl_UpdateWorldMarketDB.acquire();
	gl_dataContainerFinnhubStock.UpdateProfileDB();
	gl_UpdateWorldMarketDB.release();
	sm_fProcessing = false;

	return 37;
}
