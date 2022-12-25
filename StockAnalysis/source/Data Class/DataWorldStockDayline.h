#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CDataWorldStockDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CDataWorldStockDayLine();
	~CDataWorldStockDayLine() override = default;

public:
	bool SaveDB(const CString& strStockSymbol) final;
	bool LoadDB(const CString& strStockSymbol) final;

public:
	// ÌØÓÐº¯Êý

private:
};

using CDataWorldStockDayLinePtr = shared_ptr<CDataWorldStockDayLine>;
