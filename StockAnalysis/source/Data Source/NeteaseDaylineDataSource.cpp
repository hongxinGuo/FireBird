#include"pch.h"

#include"NeteaseDayLineDataSource.h"
#include"ProductNeteaseDayLine.h"

#include"ChinaMarket.h"

CNeteaseDayLineDataSource::CNeteaseDayLineDataSource() {
	Reset();
}

bool CNeteaseDayLineDataSource::Reset(void) {
	return true;
}

bool CNeteaseDayLineDataSource::UpdateStatus(void) {
	return true;
}

// ץȡ��������.��ʼ��11:45:01
// ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�
//
// ���ڿ��ܻ�ץȡȫ��5000�������������ݣ�����ʱ�䳬��10���ӣ��ʶ�9:15:00��һ������ϵͳʱ��ȥ���£�����9:25:00�ڶ�������ϵͳ��ſ�ʼ��
// Ϊ�˷�ֹ������ϵͳ������ͻ��ʵ��ִ��ʱ���Ӻ���11:45:01,�Ҳ�������ʵʱ���ݵĹ���ʱ��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNeteaseDayLineDataSource::Inquire(long lCurrentTime) {
	if (gl_pChinaMarket->IsSystemReady() && gl_pChinaMarket->IsDayLineNeedUpdate() && gl_pChinaMarket->IsDummyTime() && (
		gl_pChinaMarket->GetMarketTime() > 114500)) {
		if (!IsInquiring()) {
			const auto product = make_shared<CProductNeteaseDayLine>();
			StoreInquiry(product);
			SetInquiring(true);
			return true;
		}
	}
	return true;
}
