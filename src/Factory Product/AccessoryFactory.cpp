module;
#include <absl/log/absl_check.h>

module FireBirdLib.Factory.Accessory;

import FireBirdLib.Product.Dummy;
import FireBirdLib.Product.IndexNasdaq100Stocks;
import FireBirdLib.SystemMessage;
import FireBirdLib.Market;
import FireBirdLib.FinnhubInquiryType;

using std::make_shared;

CVirtualWebProductPtr CAccessoryFactory::CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) {
	CVirtualWebProductPtr p = nullptr;

	switch (iInquireType) {
	case ACCESSORY_INDEX_NASDAQ100_STOCKS_:
		p = make_shared<CProductIndexNasdaq100Stocks>();
		break;
	default:
		p = make_shared<CProductDummy>();
		ABSL_DCHECK(0) << "未处理指令";
		gl_systemMessage.PushErrorMessage("Accessory product未实现");
		break;
	}
	p->SetInquireType(iInquireType);

	return p;
}
