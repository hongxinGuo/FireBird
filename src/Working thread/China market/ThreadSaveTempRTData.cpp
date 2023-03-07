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

UINT ThreadSaveTempRTData(not_null<CChinaMarket*> pMarket) {
	ASSERT(pMarket->IsSystemReady()); // 调用本工作线程时必须设置好市场。

	gl_ProcessChinaMarketRTData.acquire();
	gl_ThreadStatus.IncreaseSavingThread();

	pMarket->UpdateTodayTempDB();

	gl_ThreadStatus.DecreaseSavingThread();
	gl_ProcessChinaMarketRTData.release();

	return 13;
}

/// <summary>
/// 此线程与存储实时数据的线程互斥。
/// </summary>
/// <param name="pMarket"></param>
/// <returns></returns>
UINT ThreadProcessRTData(not_null<CChinaMarket*> pMarket) {
	gl_ProcessChinaMarketRTData.acquire();
	pMarket->ProcessRTData();
	gl_ProcessChinaMarketRTData.release();
	return 107;
}
