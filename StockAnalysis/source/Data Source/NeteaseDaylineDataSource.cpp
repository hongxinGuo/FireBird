#include"pch.h"

#include"NeteaseDaylineDataSource.h"
#include"ProductNeteaseDayline.h"

#include"ChinaMarket.h"

CNeteaseDaylineDataSource::CNeteaseDaylineDataSource() {
	m_pWebInquiry = nullptr;
	Reset();
}

CNeteaseDaylineDataSource::~CNeteaseDaylineDataSource() {
}

bool CNeteaseDaylineDataSource::Reset(void) {
	return true;
}

bool CNeteaseDaylineDataSource::UpdateStatus(void) {
	return true;
}

// ץȡ��������.��ʼ��11:45:01
// ���ʹ���ĸ����棬�������ױ����׷������ܾ�����һ�㻹��������Ϊ�á�
//
// ���ڿ��ܻ�ץȡȫ��5000�������������ݣ�����ʱ�䳬��10���ӣ��ʶ�9:15:00��һ������ϵͳʱ��ȥ���£�����9:25:00�ڶ�������ϵͳ��ſ�ʼ��
// Ϊ�˷�ֹ������ϵͳ������ͻ��ʵ��ִ��ʱ���Ӻ���11:45:01,�Ҳ�������ʵʱ���ݵĹ���ʱ��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNeteaseDaylineDataSource::Inquire(long lCurrentTime) {
	if (gl_pChinaMarket->IsSystemReady() && gl_pChinaMarket->IsDayLineNeedUpdate() && gl_pChinaMarket->IsDummyTime() && (gl_pChinaMarket->GetMarketTime() > 114500)) {
		if (!IsInquiring()) {
			CVirtualProductWebDataPtr product = make_shared<CProductNeteaseDayline>();
			StoreInquiry(product);
			SetInquiring(true);
			return true;
		}
	}
	return true;
}