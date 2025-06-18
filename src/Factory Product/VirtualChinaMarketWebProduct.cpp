#include "pch.h"
#include "VirtualChinaMarketWebProduct.h"
#include"ChinaMarket.h"
#include "WebData.h"

void CVirtualChinaMarketWebProduct::CalculateTotalDataLength(shared_ptr<vector<CWebDataPtr>> pvWebData) {
	for (auto pWebData : *pvWebData) {
		if (pWebData != nullptr) {
			gl_ChinaMarketTotalData += pWebData->GetBufferLength();
		}
	}
}

CChinaMarketPtr CVirtualChinaMarketWebProduct::GetMarket() const noexcept {
	if (const CVirtualMarketPtr p = m_pMarket.lock()) {
		return dynamic_pointer_cast<CChinaMarket>(p);
	}
	exit(1);  // ������Ϊnullptrʱ����ʱϵͳ���߳��Ѿ��˳��ˣ������߳�ҲҪ�����˳�
}
