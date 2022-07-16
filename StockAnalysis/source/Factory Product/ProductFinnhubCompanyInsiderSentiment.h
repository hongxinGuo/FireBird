#pragma once

#include"ProductWebData.h"
#include"InsiderSentiment.h"

class CProductFinnhubCompanyInsiderSentiment : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyInsiderSentiment)
	CProductFinnhubCompanyInsiderSentiment();
	~CProductFinnhubCompanyInsiderSentiment() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CInsiderSentimentVectorPtr ParseFinnhubStockInsiderSentiment(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCompanyInsiderSentiment> CProductFinnhubCompanyInsiderSentimentPtr;
