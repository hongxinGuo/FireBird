#pragma once

#include"WorldMarket.h"
#include"ProductWebData.h"

class CProductFinnhubCryptoSymbol : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubCryptoSymbol)
	CProductFinnhubCryptoSymbol();
	~CProductFinnhubCryptoSymbol() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CFinnhubCryptoSymbolVectorPtr ParseFinnhubCryptoSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCryptoSymbol> CFinnhubCryptoSymbolProductPtr;