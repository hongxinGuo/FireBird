#pragma once

#include"ProductFinnhub.h"
#include"InsiderSentiment.h"

class CProductFinnhubCompanyInsiderSentiment final : public CProductFinnhub {
public:
	CProductFinnhubCompanyInsiderSentiment();
	~CProductFinnhubCompanyInsiderSentiment() override = default;

	CString CreateMessage(void) override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CInsiderSentimentVectorPtr ParseFinnhubStockInsiderSentiment(CWebDataPtr pWebData);
};

using CProductFinnhubCompanyInsiderSentimentPtr = shared_ptr<CProductFinnhubCompanyInsiderSentiment>;
