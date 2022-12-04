#pragma once

#include"VirtualProductWebData.h"
#include"TiingoCryptoSymbol.h"

class CProductTiingoCryptoSymbol : public CVirtualProductWebData {
public:
	DECLARE_DYNCREATE(CProductTiingoCryptoSymbol)
		CProductTiingoCryptoSymbol();
	~CProductTiingoCryptoSymbol() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData, CVirtualDataSource* pDataSource = nullptr) override final;
	CTiingoCryptoVectorPtr ParseTiingoCryptoSymbol(CWebDataPtr pWebData);

	// ≤‚ ‘”√
	long long m_differ1, m_differ2;
	double m_ratio;
};

typedef shared_ptr<CProductTiingoCryptoSymbol> CProductTiingoCryptoSymbolsPtr;
