#pragma once

#include<memory>
#include<string>

#include"ClassDeclaration.h"
#include"WebRTData.h"
#include"WebData.h"

#include"VirtualWebProduct.h"

class CProductSinaRT final : public CVirtualWebProduct {
public:
	DECLARE_DYNCREATE(CProductSinaRT)
	CProductSinaRT();

	~CProductSinaRT() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;

public:
	bool ParseSinaRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData);

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
