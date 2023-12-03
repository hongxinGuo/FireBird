/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//Tiingo����Ϊ�շ�ҵ��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ProductTiingo.h"

class CProductTiingoCompanyProfile final : public CProductTiingo {
public:
	CProductTiingoCompanyProfile();
	~CProductTiingoCompanyProfile() override = default;

	CString CreateMessage() override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
};

using CTiingoCompanyProfilePtr = shared_ptr<CProductTiingoCompanyProfile>;
