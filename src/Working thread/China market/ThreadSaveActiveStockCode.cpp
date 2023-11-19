///////////////////////////////////////////////////////////////////////////////////
//
// 存储新找到的证券代码至数据库
// 此线程调用ThreadBuildDayLineRSOfDate线程，目前最多允许同时生成8个线程。
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"Thread.h"
#include"ChinaMarket.h"

UINT ThreadSaveStockSection(not_null<CChinaMarketPtr> pMarket) {
	gl_UpdateChinaMarketDB.acquire();
	pMarket->SaveStockSection();
	gl_UpdateChinaMarketDB.release();

	return 35;
}
