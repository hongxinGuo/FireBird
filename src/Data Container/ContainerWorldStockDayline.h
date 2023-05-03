#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CContainerWorldStockDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CContainerWorldStockDayLine();
	~CContainerWorldStockDayLine() override = default;

	bool SaveDB(const CString& strStockSymbol) override;
	bool LoadDB(const CString& strStockSymbol) override;

	// ���к���
};

using CContainerWorldStockDayLinePtr = shared_ptr<CContainerWorldStockDayLine>;
