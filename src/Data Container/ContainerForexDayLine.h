#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CContainerForexDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CContainerForexDayLine();
	CContainerForexDayLine(const CContainerForexDayLine& other) = delete;
	CContainerForexDayLine(CContainerForexDayLine&& other) noexcept = delete;
	CContainerForexDayLine& operator=(const CContainerForexDayLine& other) = delete;
	CContainerForexDayLine& operator=(CContainerForexDayLine&& other) noexcept = delete;
	~CContainerForexDayLine() override = default;

	bool SaveDB(const string& strCryptoSymbol) override;
	bool LoadDB(const string& strCryptoSymbol) override;

	// ÌØÓÐº¯Êý
};

using CContainerForexDayLinePtr = shared_ptr<CContainerForexDayLine>;
