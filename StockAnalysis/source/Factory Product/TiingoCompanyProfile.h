/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo�����Ϊ�������Ŀ��Ŀǰ��δʹ�á�
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
