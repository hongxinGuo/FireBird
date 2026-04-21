#pragma once

#include"VirtualWorldMarketWebProduct.h"

class CProductAlphaVantage : public CVirtualWorldMarketWebProduct {
public:
	CProductAlphaVantage() = default;
	// 不允许赋值。
	CProductAlphaVantage(const CProductAlphaVantage&) = delete;
	CProductAlphaVantage& operator=(const CProductAlphaVantage&) = delete;
	CProductAlphaVantage(const CProductAlphaVantage&&) noexcept = delete;
	CProductAlphaVantage& operator=(const CProductAlphaVantage&&) noexcept = delete;
	~CProductAlphaVantage() override = default;
};
