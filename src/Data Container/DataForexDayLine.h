#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CDataForexDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CDataForexDayLine();
	~CDataForexDayLine() override = default;

	bool SaveDB(const CString& strCryptoSymbol) override;
	bool LoadDB(const CString& strCryptoSymbol) override;

	// ÌØÓÐº¯Êý
};

using CDataForexDayLinePtr = shared_ptr<CDataForexDayLine>;
