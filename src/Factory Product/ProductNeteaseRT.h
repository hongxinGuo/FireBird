#pragma once

#include"ClassDeclaration.h"
#include"WebRTData.h"

#include"VirtualWebProduct.h"

class CProductNeteaseRT final : public CVirtualWebProduct {
public:
	CProductNeteaseRT();

	~CProductNeteaseRT() override = default;

	CString CreateMessage(void);
	bool ParseAndStoreWebData(CWebDataPtr pWebData);

public:
	bool ParseNeteaseRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData);

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
