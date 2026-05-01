#pragma once

#include "VirtualDataHistoryCandle.h"

class CContainerForexDayLine final : public CVirtualDataHistoryCandle {
public:
	CContainerForexDayLine() = default;
	CContainerForexDayLine(const CContainerForexDayLine& other) = delete;
	CContainerForexDayLine(CContainerForexDayLine&& other) noexcept = delete;
	CContainerForexDayLine& operator=(const CContainerForexDayLine& other) = delete;
	CContainerForexDayLine& operator=(CContainerForexDayLine&& other) noexcept = delete;
	~CContainerForexDayLine() override = default;

	bool SaveDB(const string& strCryptoSymbol) override;
	bool LoadDB(const string& strCryptoSymbol) override;

	// 特有函数
};

using CContainerForexDayLinePtr = shared_ptr<CContainerForexDayLine>;
