#pragma once

#include"WebSourceDataFactory.h"

class CTiingoFactory : public CWebSourceDataFactory {
public:
	CTiingoFactory() {}
	~CTiingoFactory() {}

	virtual CWebSourceDataProductPtr CreateProduct(int iIndex) override final;
};
