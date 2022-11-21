#include"pch.h"

#include"SinaRTDataSource.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"

CSinaRTDataSource::CSinaRTDataSource() {
	m_pWebInquiry = nullptr;
	m_fPermitToConcurrentProceed = true; // ����ʵʱ����Դ���ò��д���ģʽ���ڽ�������֮ǰ�������ٴ��������ݣ��Ա������ݵ������ٶȡ�
	Reset();
}

CSinaRTDataSource::~CSinaRTDataSource() {
}

bool CSinaRTDataSource::Reset(void) {
	return true;
}

bool CSinaRTDataSource::UpdateStatus(void) {
	return true;
}

bool CSinaRTDataSource::Inquire(long lCurrentTime) {
	static long long sllLastTimeTickCount = 0;
	long long llTickCount = 0;

	llTickCount = GetTickCount64();
	if (llTickCount > (sllLastTimeTickCount + gl_systemConfigeration.GetChinaMarketRTDataInquiryTime())) {
		if (!IsInquiring()) {
			InquireRTData(lCurrentTime);
		}
		if (IsInquiring()) {
			if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady()) {
				sllLastTimeTickCount = llTickCount + 60000; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
			}
			else {
				sllLastTimeTickCount = llTickCount;
			}
		}
	}
	return true;
}

bool CSinaRTDataSource::InquireRTData(long lCurrentTime) {
	if (!IsInquiring()) {
		CVirtualProductWebDataPtr product = make_shared<CProductSinaRT>();
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}