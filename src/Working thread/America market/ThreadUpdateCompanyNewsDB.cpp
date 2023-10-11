#include"pch.h"

#include"ThreadStatus.h"
#include"WorldMarket.h"

UINT ThreadUpdateCompanyNewsDB(not_null<CWorldMarket*> pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateCompanyNewsDB();
	gl_UpdateWorldMarketDB.release();

	return 59;
}
