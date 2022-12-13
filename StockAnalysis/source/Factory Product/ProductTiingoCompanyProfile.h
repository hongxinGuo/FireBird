/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo�����Ϊ�������Ŀ��Ŀǰ��δʹ�á�
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualWebProduct.h"

class CProductTiingoCompanyProfile final : public CVirtualWebProduct {
public:
	DECLARE_DYNCREATE(CProductTiingoCompanyProfile)
	CProductTiingoCompanyProfile();
	~CProductTiingoCompanyProfile() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pData) override final;
};

typedef shared_ptr<CProductTiingoCompanyProfile> CTiingoCompanyProfilePtr;
