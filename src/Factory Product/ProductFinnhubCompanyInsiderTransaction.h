#pragma once

#include"ProductFinnhub.h"
#include"InsiderTransaction.h"

class CProductFinnhubCompanyInsiderTransaction final : public CProductFinnhub {
public:
	CProductFinnhubCompanyInsiderTransaction();
	~CProductFinnhubCompanyInsiderTransaction() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	void UpdateSystemStatus(CVirtualDataSourcePtr) override;
	CInsiderTransactionsPtr ParseFinnhubStockInsiderTransaction(const CWebDataPtr& pWebData);
};

using CFinnhubCompanyInsiderTransactionPtr = shared_ptr<CProductFinnhubCompanyInsiderTransaction>;
