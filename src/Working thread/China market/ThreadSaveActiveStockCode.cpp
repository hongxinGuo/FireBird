///////////////////////////////////////////////////////////////////////////////////
//
// 存储新找到的证券代码至数据库
// 此线程调用ThreadBuildDayLineRSOfDate线程，目前最多允许同时生成8个线程。
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

UINT ThreadSaveStockSection() {
	gl_UpdateChinaMarketDB.acquire();
	gl_dataContainerChinaStockSymbol.UpdateStockSectionDB();
	gl_UpdateChinaMarketDB.release();

	return 35;
}
