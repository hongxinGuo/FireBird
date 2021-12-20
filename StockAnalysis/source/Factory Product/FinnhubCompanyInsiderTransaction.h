#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubCompanyInsiderTransaction : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubCompanyInsiderTransaction)
	CFinnhubCompanyInsiderTransaction();
	~CFinnhubCompanyInsiderTransaction() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CInsiderTransactionVectorPtr ParseFinnhubStockInsiderTransaction(CWebDataPtr pWebData);
};

typedef shared_ptr<CFinnhubCompanyInsiderTransaction> CFinnhubCompanyInsiderTransactionPtr;