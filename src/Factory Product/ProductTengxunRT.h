#pragma once
#include"ChinaMarket.h"
import FireBird.WebProduct.VirtualChinaMarket;

class CProductTengxunRT final : public CVirtualChinaMarketWebProduct {
public:
	CProductTengxunRT();
	~CProductTengxunRT() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;

protected:
	long m_lCurrentStockPosition; // ��Ʊ��ǰ��ѯλ��
};
