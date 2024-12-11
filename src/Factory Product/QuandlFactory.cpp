#include "pch.h"
module;
module FireBird.Factory.Quandl;

import FireBird.Market.Virtual;
import FireBird.WebProduct.Virtual;
import FireBird.DataSource.Virtual;

CVirtualProductWebDataPtr CQuandlFactory::CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) {
	return CVirtualProductWebDataPtr();
}
