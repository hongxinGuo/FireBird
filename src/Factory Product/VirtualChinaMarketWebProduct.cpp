#include "pch.h"
module;
#include"ChinaMarket.h"
module FireBird.WebProduct.VirtualChinaMarket;

CChinaMarketPtr CVirtualChinaMarketWebProduct::GetMarket() const noexcept {
	if (const CVirtualMarketPtr p = m_pMarket.lock()) {
		return dynamic_pointer_cast<CChinaMarket>(p);
	}
	exit(1);  // ������Ϊnullptrʱ����ʱϵͳ���߳��Ѿ��˳��ˣ������߳�ҲҪ�����˳�
}
