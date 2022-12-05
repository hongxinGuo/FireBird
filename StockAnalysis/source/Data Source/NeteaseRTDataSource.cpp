#include"pch.h"

#include"NeteaseRTDataSource.h"
#include"ProductNeteaseRT.h"

#include"ChinaMarket.h"

CNeteaseRTDataSource::CNeteaseRTDataSource() {
	Reset();
}

CNeteaseRTDataSource::~CNeteaseRTDataSource() {
}

bool CNeteaseRTDataSource::Reset(void) {
	return true;
}

bool CNeteaseRTDataSource::UpdateStatus(void) {
	return true;
}

bool CNeteaseRTDataSource::Inquire(long lCurrentTime) {
	static long long sllLastTimeTickCount = 0;
	long long llTickCount = 0;

	llTickCount = GetTickCount64();
	if (llTickCount > (sllLastTimeTickCount + gl_systemConfigeration.GetChinaMarketRTDataInquiryTime())) {
		if (!IsInquiring()) {
			InquireRTData(lCurrentTime);
		}
		if (IsInquiring()) {
			if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfigeration.IsDebugMode()) {// ϵͳ����Ϊ������ϵͳʱ����������ѯ�ٶ�
				sllLastTimeTickCount = llTickCount + 60000; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
			}
			else {
				sllLastTimeTickCount = llTickCount;
			}
		}
	}
	return true;
}

bool CNeteaseRTDataSource::InquireRTData(long lCurrentTime) {
	if (!IsInquiring()) {
		CVirtualProductWebDataPtr product = make_shared<CProductNeteaseRT>();
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}