///////////////////////////////////////////////////////////////////////////////////
//
// 从活跃股票集中挑选强势股票。
//
//
///////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"Thread.h"

UINT TaskLoadSelectedStockHistoryData() {
	// 加载中国股票
	for (size_t index = 0; index < gl_dataContainerChinaStock.Size(); index++) {
		auto pStock = gl_dataContainerChinaStock.GetStock(index);
		if (pStock->IsSelected() && !pStock->IsDayLineLoaded()) {
			gl_runtime.thread_executor()->post([pStock] {
				// 装入日线数据
				pStock->LoadDayLine(pStock->GetSymbol());
				// 计算各相对强度（以指数相对强度为默认值）
				pStock->CalculateDayLineRSIndex();
				pStock->SetDayLineLoaded(true);
			});
			gl_runtime.thread_executor()->post([pStock] {
				// 装入周线数据
				pStock->LoadWeekLine();
				// 计算各相对强度（以指数相对强度为默认值）
				pStock->CalculateWeekLineRSIndex();
				pStock->SetWeekLineLoaded(true);
			});
		}
	}
	// 加载Tiingo股票
	for (size_t index = 0; index < gl_dataContainerTiingoStock.Size(); index++) {
		auto pStock = gl_dataContainerTiingoStock.GetStock(index);
		if (pStock->IsSelected() && !pStock->IsDayLineLoaded()) {
			gl_runtime.thread_executor()->post([pStock] {
				pStock->LoadDayLineDB();
				pStock->CreateWeekLine(); // 生成weekLine
				pStock->SetWeekLineLoaded(true);
			});
		}
	}

	return 111;
}
