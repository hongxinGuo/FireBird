/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo�����Ϊ�������Ŀ��Ŀǰ��δʹ�á�
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ProductWebData.h"

class CProductTiingoCompanyProfile : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductTiingoCompanyProfile)
	CProductTiingoCompanyProfile();
	~CProductTiingoCompanyProfile() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pData) override final;
};

typedef shared_ptr<CProductTiingoCompanyProfile> CTiingoCompanyProfilePtr;
