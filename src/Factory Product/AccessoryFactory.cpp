#include "pch.h"

#include"FinnhubInquiryType.h"

#include "AccessoryFactory.h"

#include "ProductDummy.h"
#include "ProductIndexNasdaq100Stocks.h"

CVirtualProductWebDataPtr CAccessoryFactory::CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) {
	CVirtualProductWebDataPtr p = nullptr;

	switch (iInquireType) {
	case ACCESSORY_INDEX_NASDAQ100_STOCKS:
		p = make_shared<CProductIndexNasdaq100Stocks>();
		break;
	default:
		p = make_shared<CProductDummy>();
		TRACE("未处理指令%d\n", iInquireType);
		gl_systemMessage.PushErrorMessage("Finnhub product未实现");
		break;
	}
	p->SetMarket(pMarket);
	p->SetInquireType(iInquireType);

	return p;
}
