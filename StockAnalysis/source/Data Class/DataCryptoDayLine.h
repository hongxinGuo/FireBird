#pragma once

#include"DayLine.h"
#include "VirtualDataHistoryCandleExtend.h"

class CDataCryptoDayLine : public CVirtualDataHistoryCandleExtend {
public:
	CDataCryptoDayLine();
	virtual ~CDataCryptoDayLine();
public:
	virtual bool SaveDB(CString strCryptoSymbol) override final;
	virtual bool LoadDB(CString strCryptoSymbol) override final;

public:
	// ÌØÓÐº¯Êý

private:
};

typedef shared_ptr<CDataCryptoDayLine> CDataCryptoDayLinePtr;
