///////////////////////////////////////////////////////////////////////////////////
//
// 存储新找到的证券代码至数据库
// 此线程调用ThreadBuildDayLineRSOfDate线程，目前最多允许同时生成8个线程。
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"Thread.h"

#include"ChinaMarket.h"

using namespace std;
#include<thread>

UINT ThreadSaveActiveStakeCode(not_null<CChinaMarket*> pMarket) {
  pMarket->SaveActiveStakeCode();

  return 34;
}

UINT ThreadSaveStakeSection(not_null<CChinaMarket*> pMarket) {
  pMarket->SaveStakeSection();

  return 35;
}