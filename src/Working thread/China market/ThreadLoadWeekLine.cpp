#include"pch.h"

#include"ChinaMarket.h"
#include"Thread.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 日线装载工作线程
//
// 从数据库中装入相应股票的周线数据，然后计算各相对强度
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadLoadWeekLine(not_null<CChinaStock*> pStock) {
	pStock->UnloadWeekLine();
	// 装入周线数据
	pStock->LoadWeekLine();
	// 计算各相对强度（以指数相对强度为默认值）
	pStock->CalculateWeekLineRSIndex();
	pStock->SetWeekLineLoaded(true);

	return 29;
}