#include "pch.h"
#include "VirtualChinaMarketWebProduct.h"
#include"ChinaMarket.h"

shared_ptr<CChinaMarket> CVirtualChinaMarketWebProduct::GetMarket() const noexcept {
	if (const CVirtualMarketPtr p = m_pMarket.lock()) {
		return dynamic_pointer_cast<CChinaMarket>(p);
	}
	exit(1);  // ������Ϊnullptrʱ����ʱϵͳ���߳��Ѿ��˳��ˣ������߳�ҲҪ�����˳�
}
