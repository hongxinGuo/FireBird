#pragma once

#include"ProductFinnhub.h"
#include"InsiderSentiment.h"

class CProductFinnhubCompanyInsiderSentiment final : public CProductFinnhub {
public:
	CProductFinnhubCompanyInsiderSentiment();
	~CProductFinnhubCompanyInsiderSentiment() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CInsiderSentimentVectorPtr ParseFinnhubStockInsiderSentiment(CWebDataPtr pWebData);
};

using CProductFinnhubCompanyInsiderSentimentPtr = shared_ptr<CProductFinnhubCompanyInsiderSentiment>;
