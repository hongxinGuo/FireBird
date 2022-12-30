#pragma once

class CVirtualDataFactory {
public:
	CVirtualDataFactory() = default;

	virtual ~CVirtualDataFactory() = default;

	virtual CVirtualProductWebDataPtr CreateProduct(CVirtualMarket* pMarket, int iIndex) = 0;
};
