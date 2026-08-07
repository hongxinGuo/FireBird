#pragma once

#include"ProductFinnhub.h"

class CSECFiling;

class CProductFinnhubSECFilings final : public CProductFinnhub {
public:
	CProductFinnhubSECFilings();
	~CProductFinnhubSECFilings() override = default;

	shared_ptr<vector<string>> CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<CSECFiling>> ParseFinnhubStockSECFilings(const CWebDataPtr& pWebData);
};

using CFinnhubSECFilings = shared_ptr<CProductFinnhubSECFilings>;
