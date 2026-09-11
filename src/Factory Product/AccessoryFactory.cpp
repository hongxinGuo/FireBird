#include "pch.h"

#include "AccessoryFactory.h"

#include "ProductDummy.h"
#include "SystemMessage.h"
#include "VirtualMarket.h"
#include"VirtualWebProduct.h"

using std::make_shared;

CVirtualWebProductPtr CAccessoryFactory::CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) {
	CVirtualWebProductPtr p = nullptr;

	switch (iInquireType) {
	default:
		p = make_shared<CProductDummy>();
		ABSL_DLOG(INFO) << std::format("未处理指令%d\n", iInquireType);
		gl_systemMessage.PushErrorMessage("Accessory product未实现");
		break;
	}
	p->SetInquireType(iInquireType);

	return p;
}
