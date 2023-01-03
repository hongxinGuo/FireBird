/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo�����Ϊ�������Ŀ��Ŀǰ��δʹ�á�
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualWebProduct.h"

class CProductTiingoCompanyProfile final : public CVirtualWebProduct {
public:
	CProductTiingoCompanyProfile();
	~CProductTiingoCompanyProfile() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
};

using CTiingoCompanyProfilePtr = shared_ptr<CProductTiingoCompanyProfile>;
