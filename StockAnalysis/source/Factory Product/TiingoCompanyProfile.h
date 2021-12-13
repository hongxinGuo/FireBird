/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo此项功能为非免费项目，目前尚未使用。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"WebSourceDataProduct.h"

class CTiingoCompanyProfile : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CTiingoCompanyProfile)
	CTiingoCompanyProfile();
	~CTiingoCompanyProfile() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pData) override final;
};

typedef shared_ptr<CTiingoCompanyProfile> CTiingoCompanyProfilePtr;
