#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CDataCryptoDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CDataCryptoDayLine();
	~CDataCryptoDayLine() override = default;

public:
	virtual bool SaveDB(CString strCryptoSymbol) override final;
	virtual bool LoadDB(CString strCryptoSymbol) override final;

public:
	// ÌØÓÐº¯Êý

private:
};

typedef shared_ptr<CDataCryptoDayLine> CDataCryptoDayLinePtr;
