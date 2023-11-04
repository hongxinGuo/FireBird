//////////////////////////////////////////////////////////////////////////////////////
//
// 存储临时实时数据工作线程
//
// 此线程与计算实时数据线程互斥，需要注意之。
//
// 返回值为13
//
///////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ThreadStatus.h"
#include"ChinaMarket.h"

/// <summary>
/// 
/// 此线程与计算实时数据的线程互斥
/// 
/// </summary>
/// <param name="pMarket"></param>
/// <returns></returns>
UINT ThreadSaveTempRTData(not_null<CChinaMarket*> pMarket) {
	ASSERT(pMarket->IsSystemReady()); // 调用本工作线程时必须设置好市场。

	gl_UpdateChinaMarketDB.acquire();
	gl_ProcessChinaMarketRTData.acquire();
	pMarket->SaveTempRTData();
	gl_ProcessChinaMarketRTData.release();
	gl_UpdateChinaMarketDB.release();

	return 13;
}

UINT ThreadLoadTempRTData(not_null<CChinaMarket*> pMarket, long lTheDate) {
	gl_ProcessChinaMarketRTData.acquire();
	pMarket->LoadTempRTData(lTheDate);
	gl_ProcessChinaMarketRTData.release();

	return 19;
}

/// <summary>
/// 此线程与存储实时数据的线程互斥。
///
/// 此为处理实时数据的进程，需要考虑运行速度，不再虚拟化其调用的DistributeAndCalculateRTData，故而不测试此进程
/// </summary>
/// <param name="pMarket"></param>
/// <returns></returns>
UINT ThreadDistributeAndCalculateRTData(not_null<CChinaMarket*> pMarket) {
	gl_ProcessChinaMarketRTData.acquire();
	pMarket->DistributeAndCalculateRTData();
	gl_ProcessChinaMarketRTData.release();
	return 108;
}
