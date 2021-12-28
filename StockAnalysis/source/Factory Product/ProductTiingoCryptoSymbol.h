#pragma once

#include"ProductWebData.h"

class CProductTiingoCryptoSymbol : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductTiingoCryptoSymbol)
	CProductTiingoCryptoSymbol();
	~CProductTiingoCryptoSymbol() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CTiingoCryptoVectorPtr ParseTiingoCryptoSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductTiingoCryptoSymbol> CProductTiingoCryptoSymbolsPtr;
