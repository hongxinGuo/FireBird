#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CDataForexDayLine : public CVirtualDataHistoryCandleExtend {
public:
	CDataForexDayLine();
	virtual ~CDataForexDayLine();
public:
	virtual bool SaveDB(CString strCryptoSymbol) override final;
	virtual bool LoadDB(CString strCryptoSymbol) override final;

public:
	// ÌØÓÐº¯Êý

private:
};

typedef shared_ptr<CDataForexDayLine> CDataForexDayLinePtr;
