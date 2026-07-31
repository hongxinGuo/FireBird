module;

module FireBirdLib.Factory.AlphaVantage;
import SystemMessage;

import FinnhubInquiryType;
import FireBirdLib.Product.AlphaVantageStockDayLine;
import FireBirdLib.Product.AlphaVantageStockSplits;
import FireBirdLib.Product.Dummy;

import FireBirdLib.Market;

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
		TRACE(_T("未处理指令%d\n"), iInquireType);
		gl_systemMessage.PushErrorMessage("Alpha Vantage product未实现");
		break;
	}
	p->SetInquireType(iInquireType);

	return p;
}
