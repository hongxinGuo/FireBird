#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CContainerCryptoDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CContainerCryptoDayLine();
	// 单一实例，不允许赋值
	CContainerCryptoDayLine(const CContainerCryptoDayLine& other) = delete;
	CContainerCryptoDayLine(CContainerCryptoDayLine&& other) = delete;
	CContainerCryptoDayLine& operator=(const CContainerCryptoDayLine& other) = delete;
	CContainerCryptoDayLine& operator=(CContainerCryptoDayLine&& other) = delete;
	~CContainerCryptoDayLine() override = default;

	bool SaveDB(const string& strCryptoSymbol) override;
	bool LoadDB(const string& strCryptoSymbol) override;

	// 特有函数
};

using CContainerCryptoDayLinePtr = shared_ptr<CContainerCryptoDayLine>;
