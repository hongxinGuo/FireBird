#pragma once

#include"ProductFinnhub.h"
#include"InsiderSentiment.h"

class CProductFinnhubCompanyInsiderSentiment final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyInsiderSentiment)
	CProductFinnhubCompanyInsiderSentiment();
	~CProductFinnhubCompanyInsiderSentiment() override = default;

	CString CreateMessage(void) override final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CInsiderSentimentVectorPtr ParseFinnhubStockInsiderSentiment(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCompanyInsiderSentiment> CProductFinnhubCompanyInsiderSentimentPtr;
