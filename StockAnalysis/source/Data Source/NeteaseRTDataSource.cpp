#include"pch.h"

#include"NeteaseRTDataSource.h"
#include"ProductNeteaseRT.h"

#include"ChinaMarket.h"

CNeteaseRTDataSource::CNeteaseRTDataSource() { Reset(); }

bool CNeteaseRTDataSource::Reset(void) { return true; }

bool CNeteaseRTDataSource::UpdateStatus(void) { return true; }

bool CNeteaseRTDataSource::Inquire(const long lCurrentTime) {
	const ULONGLONG llTickCount = GetTickCount64();
	if (static ULONGLONG sllLastTimeTickCount = 0; llTickCount > (sllLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime())) {
		if (!IsInquiring()) { InquireRTData(lCurrentTime); }
		if (IsInquiring()) {
			if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) {	// ϵͳ����Ϊ������ϵͳʱ����������ѯ�ٶ�
				sllLastTimeTickCount = llTickCount + 60000; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
			}
			else { sllLastTimeTickCount = llTickCount; }
		}
	}
	return true;
}

bool CNeteaseRTDataSource::InquireRTData(const long lCurrentTime) {
	if (!IsInquiring()) {
		const auto product = make_shared<CProductNeteaseRT>();
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}
