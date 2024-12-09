#include "pch.h"
module;
#include"VirtualDataSource.h"
#include"VirtualMarket.h"
module FireBird.Factory.Quandl;
import FireBird.WebProduct.Virtual;

CVirtualProductWebDataPtr CQuandlFactory::CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) {
	return CVirtualProductWebDataPtr();
}
