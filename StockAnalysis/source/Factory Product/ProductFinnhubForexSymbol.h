#pragma once

#include"ProductWebData.h"

class CFinnhubForexSymbolProduct : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CFinnhubForexSymbolProduct)
	CFinnhubForexSymbolProduct();
	~CFinnhubForexSymbolProduct() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CForexSymbolVectorPtr ParseFinnhubForexSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CFinnhubForexSymbolProduct> CFinnhubForexSymbolProductPtr;