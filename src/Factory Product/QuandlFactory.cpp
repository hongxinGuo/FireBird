#include "pch.h"

#include"VirtualMarket.h"
#include "QuandlFactory.h"

CVirtualProductWebDataPtr CQuandlFactory::CreateProduct(CVirtualMarketPtr pMarket, int iProductType) {
	return CVirtualProductWebDataPtr();
}
