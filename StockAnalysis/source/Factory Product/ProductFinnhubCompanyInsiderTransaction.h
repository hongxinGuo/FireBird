#pragma once

#include"ProductWebData.h"
#include"InsiderTransaction.h"

class CProductFinnhubCompanyInsiderTransaction : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyInsiderTransaction)
	CProductFinnhubCompanyInsiderTransaction();
	~CProductFinnhubCompanyInsiderTransaction() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CInsiderTransactionVectorPtr ParseFinnhubStockInsiderTransaction(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCompanyInsiderTransaction> CFinnhubCompanyInsiderTransactionPtr;