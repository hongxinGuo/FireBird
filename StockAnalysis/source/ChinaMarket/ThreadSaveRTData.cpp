//////////////////////////////////////////////////////////////////////////////////////
//
// 存储当前所选股票的实时数据的工作线程
//
//
// 返回值为19
//
///////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"globedef.h"
#include"ThreadStatus.h"

#include"ChinaMarket.h"

UINT ThreadSaveRTData(not_null<CChinaMarket*> pMarket) {
	ASSERT(pMarket->IsSystemReady()); // 调用本工作线程时必须设置好市场。
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->SaveRTData();
	gl_ThreadStatus.DecreaseSavingThread();

	return 19;
}