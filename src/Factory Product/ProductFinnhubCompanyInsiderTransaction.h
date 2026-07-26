#pragma once

#include"ProductFinnhub.h"

class CInsiderTransaction;

using std::shared_ptr;
using std::vector;

class CProductFinnhubCompanyInsiderTransaction final : public CProductFinnhub {
public:
	CProductFinnhubCompanyInsiderTransaction();
	~CProductFinnhubCompanyInsiderTransaction() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	void UpdateSystemStatus() override;
	shared_ptr<vector<CInsiderTransaction>> ParseFinnhubStockInsiderTransaction(const CWebDataPtr& pWebData);
};

using CFinnhubCompanyInsiderTransactionPtr = shared_ptr<CProductFinnhubCompanyInsiderTransaction>;
