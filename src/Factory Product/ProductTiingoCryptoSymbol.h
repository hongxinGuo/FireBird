#pragma once

#include"VirtualWebProduct.h"
#include"TiingoCryptoSymbol.h"

class CProductTiingoCryptoSymbol final : public CVirtualWebProduct {
public:
	CProductTiingoCryptoSymbol();
	~CProductTiingoCryptoSymbol() override = default;

	CString CreateMessage() final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoCryptoVectorPtr ParseTiingoCryptoSymbol(CWebDataPtr pWebData);

	// ≤‚ ‘”√
	long long m_differ1, m_differ2;
	double m_ratio;
};

using CProductTiingoCryptoSymbolsPtr = shared_ptr<CProductTiingoCryptoSymbol>;
