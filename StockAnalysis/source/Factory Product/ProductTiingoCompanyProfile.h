/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo�����Ϊ�������Ŀ��Ŀǰ��δʹ�á�
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualProductWebData.h"

class CProductTiingoCompanyProfile : public CVirtualProductWebData {
public:
	DECLARE_DYNCREATE(CProductTiingoCompanyProfile)
		CProductTiingoCompanyProfile();
	~CProductTiingoCompanyProfile() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pData, CVirtualDataSource* pDataSource = nullptr) override final;
};

typedef shared_ptr<CProductTiingoCompanyProfile> CTiingoCompanyProfilePtr;
