#pragma once

#include"WeekLine.h"
#include "VirtualDataHistoryCandle.h"

class CContainerChinaStockDayLine : public CVirtualDataHistoryCandle {
public:
	CContainerChinaStockDayLine();
	CContainerChinaStockDayLine(const CContainerChinaStockDayLine& other) = delete;
	CContainerChinaStockDayLine(CContainerChinaStockDayLine&& other) noexcept = delete;
	CContainerChinaStockDayLine& operator=(const CContainerChinaStockDayLine& other) = delete;
	CContainerChinaStockDayLine& operator=(CContainerChinaStockDayLine&& other) noexcept = delete;
	~CContainerChinaStockDayLine() override = default;

	void SaveDB(const string& strStockSymbol) final;
	void LoadDB(const string& strStockSymbol) final;
	void LoadDB(const string& strStockSymbol, long lStartDate);

	void SplitAdjust() override { m_bSplitAdjusted = true; } // do nothing

	CDayLine* GetData(const size_t lIndex) { return static_cast<CDayLine*>(CVirtualDataHistoryCandle::GetData(lIndex)); }
	CDayLine* GetDayLine(chrono::local_days lDate) { return static_cast<CDayLine*>(CVirtualDataHistoryCandle::GetCandle(lDate)); }

	CWeekLine CreateNewWeekLine(long& lCurrentDayLinePos);

	// 特有函数
	bool BuildWeekLine(vector<CWeekLine>& vWeekLine);
};

using CContainerChinaStockDayLinePtr = shared_ptr<CContainerChinaStockDayLine>;
