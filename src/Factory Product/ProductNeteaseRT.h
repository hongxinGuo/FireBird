#pragma once

//#include"SystemData.h"

#include"VirtualChinaMarketWebProduct.h"
#include "WebRTData.h"

class CProductNeteaseRT final : public CVirtualChinaMarketWebProduct {
public:
	CProductNeteaseRT();
	~CProductNeteaseRT() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;

public:
	shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRT(const CWebDataPtr& pWebData);

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
