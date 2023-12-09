#pragma once

#include"VirtualWorldMarketWebProduct.h"

using std::dynamic_pointer_cast;

class CProductFinnhub : public CVirtualWorldMarketWebProduct {
public:
	CProductFinnhub() = default;
	~CProductFinnhub() override = default;

	bool CheckAccessRight(CWebDataPtr pWebData) final;
	void AddInaccessibleExchange() override; // 检查是否允许申请此类数据（当使用免费账户时，数据源会限制使用其某些功能）

	bool IsValidData(const CWebDataPtr& pWebData);
};
