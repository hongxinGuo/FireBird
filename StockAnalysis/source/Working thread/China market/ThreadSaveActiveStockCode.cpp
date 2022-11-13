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

using namespace std;
#include<thread>

UINT ThreadSaveStockSection(not_null<CChinaMarket*> pMarket) {
	pMarket->SaveStockSection();

	return 35;
}