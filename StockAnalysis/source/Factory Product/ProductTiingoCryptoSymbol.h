#pragma once

#include"VirtualWebProduct.h"
#include"TiingoCryptoSymbol.h"

class CProductTiingoCryptoSymbol final : public CVirtualWebProduct {
public:
	DECLARE_DYNCREATE(CProductTiingoCryptoSymbol)
	CProductTiingoCryptoSymbol();
	~CProductTiingoCryptoSymbol() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CTiingoCryptoVectorPtr ParseTiingoCryptoSymbol(CWebDataPtr pWebData);

	// ≤‚ ‘”√
	long long m_differ1, m_differ2;
	double m_ratio;
};

typedef shared_ptr<CProductTiingoCryptoSymbol> CProductTiingoCryptoSymbolsPtr;
