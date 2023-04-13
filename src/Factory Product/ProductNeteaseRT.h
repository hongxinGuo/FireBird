#pragma once

#include"ClassDeclaration.h"
#include"WebRTData.h"

#include"VirtualWebProduct.h"

class CProductNeteaseRT final : public CVirtualWebProduct {
public:
	CProductNeteaseRT();
	~CProductNeteaseRT() override = default;

	CString CreateMessage() override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;

public:
	shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRT(const CWebDataPtr& pWebData);

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
