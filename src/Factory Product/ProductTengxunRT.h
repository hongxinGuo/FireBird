#pragma once

#include"ClassDeclaration.h"

#include"VirtualChinaMarketWebProduct.h"

class CProductTengxunRT final : public CVirtualChinaMarketWebProduct {
public:
	CProductTengxunRT();
	~CProductTengxunRT() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;

public:
	bool ParseTengxunRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData);

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
