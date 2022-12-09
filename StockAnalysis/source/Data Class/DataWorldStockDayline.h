#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CDataWorldStockDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CDataWorldStockDayLine();
	~CDataWorldStockDayLine() override = default;

public:
	virtual bool SaveDB(CString strStockSymbol) override final;
	virtual bool LoadDB(CString strStockSymbol) override final;

public:
	// ÌØÓÐº¯Êý

private:
};

typedef shared_ptr<CDataWorldStockDayLine> CDataWorldStockDayLinePtr;
