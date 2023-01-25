#pragma once
#include"ClassDeclaration.h"

#include"VirtualWebProduct.h"

class CProductTengxunDayLine final : public CVirtualWebProduct {
public:
	CProductTengxunDayLine();

	~CProductTengxunDayLine() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
