#pragma once

#include"ProductFinnhub.h"
#include"InsiderTransaction.h"

class CProductFinnhubCompanyInsiderTransaction final : public CProductFinnhub {
public:
	CProductFinnhubCompanyInsiderTransaction();
	~CProductFinnhubCompanyInsiderTransaction() override = default;

	CString CreateMessage(void) override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CInsiderTransactionVectorPtr ParseFinnhubStockInsiderTransaction(CWebDataPtr pWebData);
};

using CFinnhubCompanyInsiderTransactionPtr = shared_ptr<CProductFinnhubCompanyInsiderTransaction>;
