#pragma once

#include"ClassDeclaration.h"
#include"WebRTData.h"

#include"VirtualWebProduct.h"

class CProductSinaRT final : public CVirtualWebProduct {
public:
	CProductSinaRT();

	~CProductSinaRT() override = default;

	CString CreateMessage(void);
	bool ParseAndStoreWebData(CWebDataPtr pWebData);

public:
	bool ParseSinaRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData);

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
