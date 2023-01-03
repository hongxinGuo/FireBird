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

UINT ThreadAppendChosenStockDB(not_null<CChinaMarket*> pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->AppendChosenStockDB();
	gl_ThreadStatus.DecreaseSavingThread();
	return 22;
}
