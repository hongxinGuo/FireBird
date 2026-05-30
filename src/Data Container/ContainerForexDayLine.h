#pragma once

#include "VirtualDataHistoryCandle.h"

class CContainerForexDayLine final : public CVirtualDataHistoryCandle {
public:
	CContainerForexDayLine();
	CContainerForexDayLine(const CContainerForexDayLine& other) = delete;
	CContainerForexDayLine(CContainerForexDayLine&& other) noexcept = delete;
	CContainerForexDayLine& operator=(const CContainerForexDayLine& other) = delete;
	CContainerForexDayLine& operator=(CContainerForexDayLine&& other) noexcept = delete;
	~CContainerForexDayLine() override = default;

	void SaveDB(const string& strForexSymbol) override;
	void LoadDB(const string& strCryptoSymbol) override;

	// 特有函数
};

using CContainerForexDayLinePtr = shared_ptr<CContainerForexDayLine>;
