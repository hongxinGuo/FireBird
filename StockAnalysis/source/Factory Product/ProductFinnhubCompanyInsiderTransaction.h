#pragma once

#include"ProductFinnhub.h"
#include"InsiderTransaction.h"

class CProductFinnhubCompanyInsiderTransaction final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyInsiderTransaction)
	CProductFinnhubCompanyInsiderTransaction();
	~CProductFinnhubCompanyInsiderTransaction() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CInsiderTransactionVectorPtr ParseFinnhubStockInsiderTransaction(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCompanyInsiderTransaction> CFinnhubCompanyInsiderTransactionPtr;
