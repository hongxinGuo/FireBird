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
UINT ThreadUpdateChinaStockProfileDB() {	//
	gl_UpdateChinaMarketDB.acquire();
	gl_dataContainerChinaStock.UpdateStockProfileDB();
	gl_UpdateChinaMarketDB.release();

	return 18;
}

UINT ThreadUpdateOptionDB(const not_null<CChinaMarketPtr>& pMarket) {
	gl_UpdateChinaMarketDB.acquire();
	pMarket->UpdateOptionDB();
	gl_UpdateChinaMarketDB.release();

	return 20;
}

UINT ThreadAppendChosenStockDB(const CChinaMarketPtr& pMarket) {
	gl_UpdateChinaMarketDB.acquire();
	pMarket->AppendChosenStockDB();
	gl_UpdateChinaMarketDB.release();

	return 22;
}
