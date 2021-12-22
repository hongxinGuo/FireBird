#pragma once

#include"ProductWebData.h"

class CProductFinnhubCryptoSymbol : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubCryptoSymbol)
	CProductFinnhubCryptoSymbol();
	~CProductFinnhubCryptoSymbol() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CCryptoSymbolVectorPtr ParseFinnhubCryptoSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCryptoSymbol> CFinnhubCryptoSymbolProductPtr;