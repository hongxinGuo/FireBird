#include"pch.h"
#include"globedef.h"

#include"DayLine.h"
#include"ChinaMarket.h"

#include"Thread.h"

//////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
UINT ThreadUpdateStockCodeDB(not_null<CChinaMarket*> pMarket) {
	//
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->UpdateStockCodeDB();
	gl_ThreadStatus.DecreaseSavingThread();
	return 18;
}

UINT ThreadUpdateOptionDB(not_null<CChinaMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->UpdateOptionDB();
	gl_ThreadStatus.DecreaseSavingThread();
	return 20;
}

UINT ThreadAppendChoicedStockDB(not_null<CChinaMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->AppendChoicedStockDB();
	gl_ThreadStatus.DecreaseSavingThread();
	return 22;
}