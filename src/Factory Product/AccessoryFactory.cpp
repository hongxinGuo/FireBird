#include "pch.h"

#include "AccessoryFactory.h"

#include "ClassDeclaration.h"
#include "ProductDummy.h"
#include "SystemMessage.h"

using std::make_shared;

CVirtualProductWebDataPtr CAccessoryFactory::CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) {
	CVirtualProductWebDataPtr p = nullptr;

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
