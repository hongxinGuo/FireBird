#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubSECFilings final : public CProductFinnhub {
public:
	CProductFinnhubSECFilings();
	~CProductFinnhubSECFilings() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CSECFilingsPtr ParseFinnhubStockSECFilings(const CWebDataPtr& pWebData);
};

using CFinnhubSECFilings = shared_ptr<CProductFinnhubSECFilings>;
