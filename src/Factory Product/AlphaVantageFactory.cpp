#include "pch.h"

#include "AlphaVantageFactory.h"

#include "FinnhubInquiryType.h"
#include "ProductAlphaVantageStockDayLine.h"
#include "ProductAlphaVantageStockSplits.h"
#include "ProductDummy.h"

CVirtualProductWebDataPtr CAlphaVantageFactory::CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) {
	CVirtualProductWebDataPtr p = nullptr;

	switch (iInquireType) {
	case STOCK_PRICE_SPLITS_:
		p = make_shared<CProductAlphaVantageStockSplits>();
		break;
	case STOCK_PRICE_CANDLES_:
		p = make_shared<CProductAlphaVantageStockDayLine>();
		break;
	default:
		p = make_shared<CProductDummy>();
		TRACE(_T("未处理指令%d\n"), iInquireType);
		gl_systemMessage.PushErrorMessage("Alpha Vantage product未实现");
		break;
	}
	p->SetInquireType(iInquireType);

	return p;
}
