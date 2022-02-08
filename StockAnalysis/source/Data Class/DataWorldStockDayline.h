#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CDataWorldStockDayLine : public CVirtualDataHistoryCandleExtend {
public:
	CDataWorldStockDayLine();
	virtual ~CDataWorldStockDayLine();
public:
	virtual bool SaveDB(CString strStockSymbol) override final;
	virtual bool LoadDB(CString strStockSymbol) override final;

public:
	// ÌØÓÐº¯Êý

private:
};

typedef shared_ptr<CDataWorldStockDayLine> CDataWorldStockDayLinePtr;
