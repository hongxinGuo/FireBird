#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

#include"WorldStock.h"

UINT ThreadUpdateCompanyNewsDB(not_null<CWorldStock*> pStock) {
	gl_UpdateWorldMarketDB.acquire();
	gl_ThreadStatus.IncreaseSavingThread();
	pStock->UpdateCompanyNewsDB();
	gl_ThreadStatus.DecreaseSavingThread();
	gl_UpdateWorldMarketDB.release();

	return 59;
}