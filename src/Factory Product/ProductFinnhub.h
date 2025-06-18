#pragma once

#include"VirtualWorldMarketWebProduct.h"

using std::dynamic_pointer_cast;

class CProductFinnhub : public CVirtualWorldMarketWebProduct {
public:
	CProductFinnhub() = default;
	~CProductFinnhub() override = default;

	void CalculateTotalDataLength(shared_ptr<vector<CWebDataPtr>> pvWebData) final;

	void AddInaccessibleSymbol() override; // 检查是否允许申请此类数据（当使用免费账户时，数据源会限制使用其某些功能）

	// 测试用函数
	bool __Test_checkAccessRight(CWebDataPtr pWebData) final; // todo 不再使用，准备删除之. 目前只用于测试函数中。
};
