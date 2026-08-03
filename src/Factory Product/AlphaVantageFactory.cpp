module;
#include <absl/log/absl_check.h>

module FireBirdLib.Factory.AlphaVantage;

import FireBirdLib.FinnhubInquiryType;
import FireBirdLib.Product.Dummy;

import FireBirdLib.Market;
import FireBirdLib.Product.AlphaVantage.StockSplits;
import FireBirdLib.Product.AlphaVantage.StockDayLine;
import FireBirdLib.SystemMessage;

import std;
using std::make_shared;

CVirtualWebProductPtr CAlphaVantageFactory::CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) {
	CVirtualWebProductPtr p = nullptr;

	switch (iInquireType) {
	case STOCK_PRICE_SPLITS_:
		p = make_shared<CProductAlphaVantageStockSplits>();
		break;
	case STOCK_PRICE_CANDLES_:
		p = make_shared<CProductAlphaVantageStockDayLine>();
		break;
	default:
		p = make_shared<CProductDummy>();
		ABSL_DCHECK(0) << "未处理指令";
		gl_systemMessage.PushErrorMessage("Alpha Vantage product未实现");
		break;
	}
	p->SetInquireType(iInquireType);

	return p;
}
