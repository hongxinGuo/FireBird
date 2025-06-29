#pragma once

#include"VirtualWebProduct.h"

class CProductDummy final : public CVirtualWebProduct {
public:
	CProductDummy();
	~CProductDummy() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr) override {}
};

using CProductDummyPtr = shared_ptr<CProductDummy>;
