#include "pch.h"
#include "VirtualChinaMarketWebProduct.h"
#include"ChinaMarket.h"
#include "WebData.h"

void CVirtualChinaMarketWebProduct::CalculateTotalDataLength(shared_ptr<vector<CWebDataPtr>> pvWebData) {
	for (const auto& pWebData : *pvWebData) {
		if (pWebData != nullptr) {
			gl_ChinaMarketTotalData += pWebData->GetBufferLength();
		}
	}
}
