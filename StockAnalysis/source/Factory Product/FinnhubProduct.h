#pragma once

#include"WebSourceProduct.h"

class CFinnhubProduct : public CWebSourceDataProduct {
public:
	CFinnhubProduct() {}
	~CFinnhubProduct() {}
};

typedef shared_ptr<CFinnhubProduct> CFinnhubProductPtr;