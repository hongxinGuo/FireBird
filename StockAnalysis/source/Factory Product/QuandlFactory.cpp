#include "pch.h"

#include"VirtualMarket.h"
#include "QuandlFactory.h"

CProductWebSourceDataPtr CQuandlFactory::CreateProduct(CVirtualMarket* pMarket, int iIndex) {
	return CProductWebSourceDataPtr();
}