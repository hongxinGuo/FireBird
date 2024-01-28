#pragma once

#include"ProductFinnhub.h"
#include"InsiderTransaction.h"

class CProductFinnhubCompanyInsiderTransaction final : public CProductFinnhub {
public:
	CProductFinnhubCompanyInsiderTransaction();
	~CProductFinnhubCompanyInsiderTransaction() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CInsiderTransactionVectorPtr ParseFinnhubStockInsiderTransaction(const CWebDataPtr& pWebData);
};

using CFinnhubCompanyInsiderTransactionPtr = shared_ptr<CProductFinnhubCompanyInsiderTransaction>;
