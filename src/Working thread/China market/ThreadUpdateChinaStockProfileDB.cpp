#include"pch.h"

#include"ThreadStatus.h"
#include"ChinaMarket.h"
#include"Thread.h"

//////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
UINT ThreadUpdateChinaStockProfileDB(not_null<CChinaMarket*> pMarket) {	//
	gl_UpdateChinaMarketDB.acquire();
	pMarket->UpdateStockProfileDB();
	gl_UpdateChinaMarketDB.release();

	return 18;
}

UINT ThreadUpdateOptionDB(not_null<CChinaMarket*> pMarket) {
	gl_UpdateChinaMarketDB.acquire();
	pMarket->UpdateOptionDB();
	gl_UpdateChinaMarketDB.release();

	return 20;
}

UINT ThreadAppendChosenStockDB(not_null<CChinaMarket*> pMarket) {
	gl_UpdateChinaMarketDB.acquire();
	pMarket->AppendChosenStockDB();
	gl_UpdateChinaMarketDB.release();

	return 22;
}
