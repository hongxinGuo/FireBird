#include "pch.h"
#include"VirtualWebProduct.h"
#include"VirtualMarket.h"

import FireBird.Factory.Quandl;

CVirtualProductWebDataPtr CQuandlFactory::CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) {
	return CVirtualProductWebDataPtr();
}
