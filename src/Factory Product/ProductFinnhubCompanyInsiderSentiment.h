#pragma once

#include"ProductFinnhub.h"
#include"InsiderSentiment.h"

class CProductFinnhubCompanyInsiderSentiment final : public CProductFinnhub {
public:
	CProductFinnhubCompanyInsiderSentiment();
	~CProductFinnhubCompanyInsiderSentiment() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CInsiderSentimentVectorPtr ParseFinnhubStockInsiderSentiment(const CWebDataPtr& pWebData);
};

using CProductFinnhubCompanyInsiderSentimentPtr = shared_ptr<CProductFinnhubCompanyInsiderSentiment>;
