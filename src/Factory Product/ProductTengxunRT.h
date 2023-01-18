#pragma once

#include"ClassDeclaration.h"
#include"WebRTData.h"

#include"VirtualWebProduct.h"

class CProductTengxunRT final : public CVirtualWebProduct {
public:
	CProductTengxunRT();

	~CProductTengxunRT() override = default;

	CString CreateMessage(void);
	bool ParseAndStoreWebData(CWebDataPtr pWebData);

public:
	bool ParseTengxunRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData);

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
