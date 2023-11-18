#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubForexExchange final : public CProductFinnhub {
public:
	CProductFinnhubForexExchange();
	~CProductFinnhubForexExchange() override = default;

	CString CreateMessage() override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<CString>> ParseFinnhubForexExchange(const CWebDataPtr& pWebData);

	void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CFinnhubForexExchangePtr = shared_ptr<CProductFinnhubForexExchange>;
