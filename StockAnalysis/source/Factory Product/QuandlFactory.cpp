#include "pch.h"

#include"VirtualMarket.h"
#include "QuandlFactory.h"

CWebSourceDataProductPtr CQuandlFactory::CreateProduct(CVirtualMarket* pMarket, int iIndex) {
	return CWebSourceDataProductPtr();
}