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
				pStock->LoadDayLineDB();
				pStock->LoadWeekLineDB();
			});
		}
	}
	// 加载Tiingo股票
	for (size_t index = 0; index < gl_dataContainerTiingoStock.Size(); index++) {
		auto pStock = gl_dataContainerTiingoStock.GetStock(index);
		if (pStock->IsSelected() && !pStock->IsDayLineLoaded()) {
			gl_runtime.thread_executor()->post([pStock] {
				pStock->LoadDayLineDB();
				pStock->LoadWeekLineDB(); // 生成weekLine
			});
		}
	}

	return 111;
}
