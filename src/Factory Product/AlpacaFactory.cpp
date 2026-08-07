#include "pch.h"

#include "SystemMessage.h"

#include "FinnhubInquiryType.h"
#include "ProductAlpacaStockDayLine.h"
#include "ProductDummy.h"
#include "AlpacaFactory.h"

using std::make_shared;

CVirtualProductWebDataPtr CAlpacaFactory::CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) {
	CVirtualProductWebDataPtr p = nullptr;

	switch (iInquireType) {
	case STOCK_PRICE_CANDLES_:
		p = make_shared<CProductAlpacaStockDayLine>();
		break;
	default:
		p = make_shared<CProductDummy>();
		ABSL_DLOG(INFO) << std::format("未处理指令%d\n", iInquireType);
		gl_systemMessage.PushErrorMessage("Alpha Vantage product未实现");
		break;
	}
	p->SetInquireType(iInquireType);

	return p;
}
