#pragma once

#include"VirtualWorldMarketWebProduct.h"

class CProductAlpaca : public CVirtualWorldMarketWebProduct {
public:
	CProductAlpaca() = default;
	// 不允许赋值。
	CProductAlpaca(const CProductAlpaca&) = delete;
	CProductAlpaca& operator=(const CProductAlpaca&) = delete;
	CProductAlpaca(const CProductAlpaca&&) noexcept = delete;
	CProductAlpaca& operator=(const CProductAlpaca&&) noexcept = delete;
	~CProductAlpaca() override = default;
};
