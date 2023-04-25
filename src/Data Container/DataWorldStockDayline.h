#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CDataWorldStockDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CDataWorldStockDayLine();
	~CDataWorldStockDayLine() override = default;

	bool SaveDB(const CString& strStockSymbol) override;
	bool LoadDB(const CString& strStockSymbol) override;

	// ���к���
};

using CDataWorldStockDayLinePtr = shared_ptr<CDataWorldStockDayLine>;
