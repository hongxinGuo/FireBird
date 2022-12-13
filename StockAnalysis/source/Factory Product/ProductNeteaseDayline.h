#pragma once

#include"ClassDeclaration.h"
#include"WebRTData.h"
#include"WebData.h"

#include"VirtualWebProduct.h"

class CProductNeteaseDayLine final : public CVirtualWebProduct {
public:
	DECLARE_DYNCREATE(CProductNeteaseDayLine)
	CProductNeteaseDayLine();

	~CProductNeteaseDayLine() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
