#pragma once

#include"ClassDeclaration.h"

#include"VirtualWebProduct.h"

class CProductTengxunRT final : public CVirtualWebProduct {
public:
	CProductTengxunRT();
	~CProductTengxunRT() override = default;

	CString CreateMessage() override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;

public:
	bool ParseTengxunRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData);

protected:
	long m_lCurrentStockPosition; // ��Ʊ��ǰ��ѯλ��
};
