#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CDataWorldStockDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CDataWorldStockDayLine();
	~CDataWorldStockDayLine() override = default;

	bool SaveDB(const CString& strStockSymbol) override;
	bool LoadDB(const CString& strStockSymbol) override;

	// ÌØÓÐº¯Êý
};

using CDataWorldStockDayLinePtr = shared_ptr<CDataWorldStockDayLine>;
