#pragma once

#include"DayLine.h"

class CDayLineWebData final {
public:
	CDayLineWebData();
	~CDayLineWebData() = default;
	void Reset();

	void ClearDayLine() noexcept { m_vTempDayLine.clear(); }
	std::vector<CDayLine>& GetProcessedDayLine() { return m_vTempDayLine; }
	void AppendDayLine(const CDayLine& data) { m_vTempDayLine.push_back(data); }

	void SetStockCode(const string& strSymbol) noexcept { m_strStockCode = strSymbol; }
	string GetStockCode() { return m_strStockCode; }

	bool TransferWebDataToBuffer(const CWebDataPtr& pWebData);
	string GetBuffer() { return m_sDataBuffer; }

	INT64 GetCurrentPos() const noexcept { return m_lCurrentPos; }
	void SetCurrentPos(const INT64 lValue) noexcept { m_lCurrentPos = lValue; }

private:
	string m_strStockCode{};
	string m_sDataBuffer{}; // 日线读取缓冲区
	vector<CDayLine> m_vTempDayLine{}; // 日线数据缓冲区
	INT64 m_lCurrentPos{ 0 };
};

using CDayLineWebDataPtr = shared_ptr<CDayLineWebData>;
