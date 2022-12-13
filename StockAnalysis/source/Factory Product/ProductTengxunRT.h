#pragma once

#include<memory>

#include"ClassDeclaration.h"
#include"WebRTData.h"
#include"WebData.h"

#include"VirtualWebProduct.h"

class CProductTengxunRT final : public CVirtualWebProduct {
public:
	DECLARE_DYNCREATE(CProductTengxunRT)
	CProductTengxunRT();

	~CProductTengxunRT() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;

public:
	bool ParseTengxunRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData);

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
