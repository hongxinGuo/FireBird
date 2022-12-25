#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CDataCryptoDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CDataCryptoDayLine();
	~CDataCryptoDayLine() override = default;

public:
	bool SaveDB(const CString& strCryptoSymbol) final;
	bool LoadDB(const CString& strCryptoSymbol) final;

public:
	// ���к���

private:
};

using CDataCryptoDayLinePtr = shared_ptr<CDataCryptoDayLine>;
