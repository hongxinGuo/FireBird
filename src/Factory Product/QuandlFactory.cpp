#include "pch.h"

#include"VirtualMarket.h"
#include "QuandlFactory.h"

CVirtualProductWebDataPtr CQuandlFactory::CreateProduct(CVirtualMarket* pMarket, int iIndex) {
	return CVirtualProductWebDataPtr();
}