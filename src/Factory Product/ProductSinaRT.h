#pragma once
#include"ChinaMarket.h"
import FireBird.WebProduct.VirtualChinaMarket;

class CProductSinaRT final : public CVirtualChinaMarketWebProduct {
public:
	CProductSinaRT();
	~CProductSinaRT() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;

protected:
	long m_lCurrentStockPosition; // ��Ʊ��ǰ��ѯλ��
};
