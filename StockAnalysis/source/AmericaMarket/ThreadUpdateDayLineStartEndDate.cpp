#include"pch.h"

#include"ThreadStatus.h"

#include"thread.h"
#include"WorldMarket.h"

UINT ThreadUpdateWorldStockDayLineStartEndDate(CWorldMarket* pMarket) {
	gl_ThreadStatus.IncreaseSavingThread();
	pMarket->UpdateStockDayLineStartEndDate();
	gl_ThreadStatus.DecreaseSavingThread();

	return 43;
}