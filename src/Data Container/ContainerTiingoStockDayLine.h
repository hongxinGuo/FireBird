#pragma once

#include "SetTiingoStockDayLine.h"
#include "TiingoDayLine.h"

class CContainerTiingoStockDayLine {
public:
	CContainerTiingoStockDayLine();
	CContainerTiingoStockDayLine(const CContainerTiingoStockDayLine& other) = delete;
	CContainerTiingoStockDayLine(CContainerTiingoStockDayLine&& other) noexcept = delete;
	CContainerTiingoStockDayLine& operator=(const CContainerTiingoStockDayLine& other) = delete;
	CContainerTiingoStockDayLine& operator=(CContainerTiingoStockDayLine&& other) noexcept = delete;
	virtual ~CContainerTiingoStockDayLine() = default;

	bool SaveDB(const string& strStockSymbol);
	bool LoadDB(const string& strStockSymbol);

	auto Size() const noexcept { return m_vHistoryData.size(); }
	bool Empty() const noexcept { return m_vHistoryData.empty(); }
	void UpdateDB(CSetTiingoStockDayLine* pSetTiingoStockDayLine, const string& strStockSymbol);
	bool UpdateDB2(CSetTiingoStockDayLine* pSetTiingoStockDayLine, const string& strStockSymbol) const;
	bool LoadBasicDB(CSetTiingoStockDayLine* pSetHistoryCandleBasic);
	bool GetStartEndDate(long& lStartDate, long& lEndDate) const;

	void UpdateData(CTiingoDayLinesPtr pvTempDayLine);
	CTiingoDayLinePtr GetData(const size_t lIndex) const {
		ASSERT(lIndex < Size());
		return m_vHistoryData.at(lIndex);
	}
	bool HaveDayLine(long lDate);
	CTiingoDayLinePtr GetDayLine(long lDate);

	bool Add(const CTiingoDayLinePtr& pData) {
		m_vHistoryData.push_back(pData);
		return true;
	}

	bool IsDayLineLoaded() const noexcept { return m_fDataLoaded.load(); }
	void SetDayLineLoaded(bool fFlag) noexcept { m_fDataLoaded = fFlag; }

	void Unload() noexcept {
		m_vHistoryData.clear();
		m_fDataLoaded = false;
	}

	// 特有函数
	void CalculateMA(int length) const;

protected:
	vector<CTiingoDayLinePtr> m_vHistoryData;
	atomic_bool m_fDataLoaded{ false }; // 数据装载与否标识
};

using CContainerTiingoStockDayLinePtr = shared_ptr<CContainerTiingoStockDayLine>;
