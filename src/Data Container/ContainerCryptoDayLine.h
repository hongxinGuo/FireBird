#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CContainerCryptoDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CContainerCryptoDayLine();
	~CContainerCryptoDayLine() override = default;

	bool SaveDB(const CString& strCryptoSymbol) override;
	bool LoadDB(const CString& strCryptoSymbol) override;

	// ÌØÓÐº¯Êý
};

using CContainerCryptoDayLinePtr = shared_ptr<CContainerCryptoDayLine>;
