#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CDataCryptoDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CDataCryptoDayLine();
	~CDataCryptoDayLine() override = default;

	bool SaveDB(const CString& strCryptoSymbol) override;
	bool LoadDB(const CString& strCryptoSymbol) override;

	// ÌØÓÐº¯Êý
};

using CDataCryptoDayLinePtr = shared_ptr<CDataCryptoDayLine>;
