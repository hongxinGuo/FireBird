#pragma once

#include"ProductFinnhub.h"

class CInsiderSentiment;

using std::shared_ptr;
using std::vector;

class CProductFinnhubCompanyInsiderSentiment final : public CProductFinnhub {
public:
	CProductFinnhubCompanyInsiderSentiment();
	~CProductFinnhubCompanyInsiderSentiment() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<CInsiderSentiment>> ParseFinnhubStockInsiderSentiment(const CWebDataPtr& pWebData);
};

using CProductFinnhubCompanyInsiderSentimentPtr = shared_ptr<CProductFinnhubCompanyInsiderSentiment>;
