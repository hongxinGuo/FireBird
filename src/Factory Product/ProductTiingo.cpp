#include"pch.h"

#include"ProductTiingo.h"
#include "WorldMarket.h"

shared_ptr<CWorldMarket> CProductTiingo::GetMarket() const noexcept {
	if (CVirtualMarketPtr p = m_pMarket.lock()) {
		return dynamic_pointer_cast<CWorldMarket>(p);
	}
	//exit(1);  // todo ������Ϊnullptrʱ����ʱϵͳ���߳��Ѿ��˳��ˣ������߳�ҲҪ�����˳�
	return nullptr;
}
