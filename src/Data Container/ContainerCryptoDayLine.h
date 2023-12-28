#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CContainerCryptoDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CContainerCryptoDayLine();
	// ��һʵ����������ֵ
	CContainerCryptoDayLine(const CContainerCryptoDayLine& other) = delete;
	CContainerCryptoDayLine(CContainerCryptoDayLine&& other) = delete;
	CContainerCryptoDayLine& operator=(const CContainerCryptoDayLine& other) = delete;
	CContainerCryptoDayLine& operator=(CContainerCryptoDayLine&& other) = delete;
	~CContainerCryptoDayLine() override = default;

	bool SaveDB(const CString& strCryptoSymbol) override;
	bool LoadDB(const CString& strCryptoSymbol) override;

	// ���к���
};

using CContainerCryptoDayLinePtr = shared_ptr<CContainerCryptoDayLine>;
