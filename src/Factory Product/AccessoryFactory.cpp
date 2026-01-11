#include "pch.h"

#include"FinnhubInquiryType.h"

#include "AccessoryFactory.h"

#include "ProductDummy.h"
#include "ProductIndexNasdaq100Stocks.h"

CVirtualProductWebDataPtr CAccessoryFactory::CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) {
	CVirtualProductWebDataPtr p = nullptr;

	switch (iInquireType) {
	case ACCESSORY_INDEX_NASDAQ100_STOCKS_:
		p = make_shared<CProductIndexNasdaq100Stocks>();
		break;
	default:
		p = make_shared<CProductDummy>();
		TRACE(_T("未处理指令%d\n"), iInquireType);
		gl_systemMessage.PushErrorMessage("Accessory product未实现");
		break;
	}
	p->SetInquireType(iInquireType);

	return p;
}
