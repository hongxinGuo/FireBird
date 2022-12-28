#pragma once

#include"ClassDeclaration.h"

#include"VirtualWebProduct.h"

class CProductFinnhub : public CVirtualWebProduct {
public:
	DECLARE_DYNCREATE(CProductFinnhub)

	CProductFinnhub() { }

	~CProductFinnhub() override = default;

	bool CheckNoRightToAccess(CWebDataPtr pWebData) final;
	bool AddInaccessibleExchangeIfNeeded(void) override; // 检查是否允许申请此类数据（当使用免费账户时，数据源会限制使用其某些功能）

protected:
};
