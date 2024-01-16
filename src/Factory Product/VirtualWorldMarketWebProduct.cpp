#include "pch.h"

#include "VirtualWorldMarketWebProduct.h"
#include"WorldMarket.h"

using std::dynamic_pointer_cast;

CWorldMarketPtr CVirtualWorldMarketWebProduct::GetMarket() const noexcept {
	if (const CVirtualMarketPtr p = m_pMarket.lock()) {
		return dynamic_pointer_cast<CWorldMarket>(p);
	}
	exit(1);  // ������Ϊnullptrʱ����ʱϵͳ���߳��Ѿ��˳��ˣ������߳�ҲҪ�����˳�
	// return nullptr;
}
