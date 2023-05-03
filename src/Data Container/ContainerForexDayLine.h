#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CContainerForexDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CContainerForexDayLine();
	~CContainerForexDayLine() override = default;

	bool SaveDB(const CString& strCryptoSymbol) override;
	bool LoadDB(const CString& strCryptoSymbol) override;

	// ���к���
};

using CContainerForexDayLinePtr = shared_ptr<CContainerForexDayLine>;
