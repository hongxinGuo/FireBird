#include"pch.h"

#include"WorldMarket.h"

UINT ThreadUpdateCompanyNewsDB(const not_null<CWorldMarketPtr>& pMarket) {
	gl_UpdateWorldMarketDB.acquire();
	pMarket->UpdateCompanyNewsDB();
	gl_UpdateWorldMarketDB.release();

	return 59;
}
