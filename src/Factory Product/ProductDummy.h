#pragma once

#include"VirtualWebProduct.h"

class CProductDummy final : public CVirtualWebProduct {
public:
	CProductDummy();
	// 不允许赋值。
	CProductDummy(const CProductDummy&) = delete;
	CProductDummy& operator=(const CProductDummy&) = delete;
	CProductDummy(const CProductDummy&&) noexcept = delete;
	CProductDummy& operator=(const CProductDummy&&) noexcept = delete;
	~CProductDummy() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr) override {}
};

using CProductDummyPtr = shared_ptr<CProductDummy>;
