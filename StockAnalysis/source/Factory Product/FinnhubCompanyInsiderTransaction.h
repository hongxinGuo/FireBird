#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubCompanyInsiderTransaction : public CWebSourceDataProduct {
public:
	CFinnhubCompanyInsiderTransaction();
	~CFinnhubCompanyInsiderTransaction() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubCompanyInsiderTransaction> CFinnhubCompanyInsiderTransactionPtr;