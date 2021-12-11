#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubCompanyProfileConcise : public CWebSourceDataProduct {
public:
	CFinnhubCompanyProfileConcise();
	~CFinnhubCompanyProfileConcise() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubCompanyProfileConcise> CFinnhubCompanyProfileConcisePtr;