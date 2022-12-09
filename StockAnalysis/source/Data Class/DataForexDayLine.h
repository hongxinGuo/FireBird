#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CDataForexDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CDataForexDayLine();
	~CDataForexDayLine() override = default;

public:
	virtual bool SaveDB(CString strCryptoSymbol) override final;
	virtual bool LoadDB(CString strCryptoSymbol) override final;

public:
	// ���к���

private:
};

typedef shared_ptr<CDataForexDayLine> CDataForexDayLinePtr;
