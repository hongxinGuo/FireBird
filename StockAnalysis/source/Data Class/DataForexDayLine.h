#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CDataForexDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CDataForexDayLine();
	~CDataForexDayLine() override = default;

public:
	bool SaveDB(const CString& strCryptoSymbol) final;
	bool LoadDB(const CString& strCryptoSymbol) final;

public:
	// ÌØÓÐº¯Êý

private:
};

using CDataForexDayLinePtr = shared_ptr<CDataForexDayLine>;
