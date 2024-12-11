#include "pch.h"
module;
#include"VirtualMarket.h"
module FireBird.Factory.Quandl;
import FireBird.WebProduct.Virtual;
import FireBird.DataSource.VirtualBase;

CVirtualProductWebDataPtr CQuandlFactory::CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) {
	return CVirtualProductWebDataPtr();
}
