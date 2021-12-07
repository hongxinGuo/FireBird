#pragma once

#include"WebSourceProduct.h"

class CTiingoProduct : public CWebSourceDataProduct {
public:
	CTiingoProduct() {}
	~CTiingoProduct() {}
};

typedef shared_ptr<CTiingoProduct> CTiingoProductPtr;
