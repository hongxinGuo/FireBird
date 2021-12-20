#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubForexExchange : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubForexExchange)
	CFinnhubForexExchange();
	~CFinnhubForexExchange() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	shared_ptr<vector<CString>> ParseFinnhubForexExchange(CWebDataPtr pWebData);
};

typedef shared_ptr<CFinnhubForexExchange> CFinnhubForexExchangePtr;