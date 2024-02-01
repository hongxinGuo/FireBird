#pragma once

#include"VirtualChinaMarketWebProduct.h"

class CProductSinaRT final : public CVirtualChinaMarketWebProduct {
public:
	CProductSinaRT();
	~CProductSinaRT() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;

public:
	bool ParseSinaRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData);

protected:
	long m_lCurrentStockPosition; // ��Ʊ��ǰ��ѯλ��
};
