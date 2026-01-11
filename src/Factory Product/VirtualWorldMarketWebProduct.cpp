#include "pch.h"

#include "VirtualWorldMarketWebProduct.h"

#include "WebData.h"

bool CVirtualWorldMarketWebProduct::IsValidData(const CWebDataPtr& pWebData) {
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = VOID_DATA_;
		return false;
	}
	if (IsNoRightToAccess()) {
		return false;
	}
	return true;
}
