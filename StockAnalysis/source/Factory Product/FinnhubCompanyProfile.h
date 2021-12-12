#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubCompanyProfile : public CWebSourceDataProduct {
public:
	CFinnhubCompanyProfile();
	~CFinnhubCompanyProfile() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubCompanyProfile> CFinnhubCompanyProfilePtr;