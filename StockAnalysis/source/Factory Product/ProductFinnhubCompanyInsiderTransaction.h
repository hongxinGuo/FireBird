#pragma once

#include"ProductFinnhub.h"
#include"InsiderTransaction.h"

class CProductFinnhubCompanyInsiderTransaction final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyInsiderTransaction)
	CProductFinnhubCompanyInsiderTransaction();
	~CProductFinnhubCompanyInsiderTransaction() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CInsiderTransactionVectorPtr ParseFinnhubStockInsiderTransaction(CWebDataPtr pWebData);
};

using CFinnhubCompanyInsiderTransactionPtr = shared_ptr<CProductFinnhubCompanyInsiderTransaction>;
