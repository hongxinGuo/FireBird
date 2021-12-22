#pragma once

#include"ProductWebData.h"

class CProductFinnhubForexExchange : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubForexExchange)
	CProductFinnhubForexExchange();
	~CProductFinnhubForexExchange() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	shared_ptr<vector<CString>> ParseFinnhubForexExchange(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubForexExchange> CFinnhubForexExchangePtr;