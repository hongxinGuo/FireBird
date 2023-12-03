#pragma once

#include"ClassDeclaration.h"

#include"VirtualWebProduct.h"
//#include "WorldMarket.h"

using std::dynamic_pointer_cast;

class CProductFinnhub : public CVirtualWebProduct {
public:
	CProductFinnhub() = default;
	~CProductFinnhub() override = default;

	[[nodiscard]] shared_ptr<CWorldMarket> GetMarket() const noexcept;
	bool CheckNoRightToAccess(CWebDataPtr pWebData) final;
	void AddInaccessibleExchange() override; // 检查是否允许申请此类数据（当使用免费账户时，数据源会限制使用其某些功能）
};
