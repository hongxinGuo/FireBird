#include "pch.h"

#include "VirtualWorldMarketWebProduct.h"
#include"WorldMarket.h"

using std::dynamic_pointer_cast;

shared_ptr<CWorldMarket> CVirtualWorldMarketWebProduct::GetMarket() const noexcept {
	if (CVirtualMarketPtr p = m_pMarket.lock()) {
		return dynamic_pointer_cast<CWorldMarket>(p);
	}
	//exit(1);  // todo ������Ϊnullptrʱ����ʱϵͳ���߳��Ѿ��˳��ˣ������߳�ҲҪ�����˳�
	return nullptr;
}
