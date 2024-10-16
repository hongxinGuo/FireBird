#pragma once

#include"VirtualWorldMarketWebProduct.h"

class CProductTiingo : public CVirtualWorldMarketWebProduct {
public:
	CProductTiingo() = default;
	~CProductTiingo() override = default;

	bool CheckAccessRight(CWebDataPtr pWebData) final; // todo 不再使用，准备删除之. 目前只用于测试函数中。
	void AddInaccessibleSymbol() override; // 检查是否允许申请此类数据（当使用免费账户时，数据源会限制使用其某些功能）
};
