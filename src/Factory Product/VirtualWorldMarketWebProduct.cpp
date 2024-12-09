#include "pch.h"
#include"WorldMarket.h"

module FireBird.WebProduct.VirtualWorldMarket;

CWorldMarketPtr CVirtualWorldMarketWebProduct::GetMarket() const noexcept {
	if (const CVirtualMarketPtr p = m_pMarket.lock()) {
		return dynamic_pointer_cast<CWorldMarket>(p);
	}
	exit(1);  // ������Ϊnullptrʱ����ʱϵͳ���߳��Ѿ��˳��ˣ������߳�ҲҪ�����˳�
	// return nullptr;
}

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
