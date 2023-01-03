#include"pch.h"

#include"SinaRTDataSource.h"
#include"SinaRTWebInquiry.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"

CSinaRTDataSource::CSinaRTDataSource() { Reset(); }

bool CSinaRTDataSource::Reset(void) { return true; }

bool CSinaRTDataSource::UpdateStatus(void) { return true; }

bool CSinaRTDataSource::Inquire(const long lCurrentTime) {
	const long long llTickCount = GetTickCount64();
	if (static long long sllLastTimeTickCount = 0; llTickCount > (sllLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime())) {
		if (!IsInquiring()) {
			InquireRTData(lCurrentTime);
		}
		if (m_pWebInquiry->IsWebError()) {
			sllLastTimeTickCount = llTickCount + 30000; //������ִ���ʱ���ӳ���ʮ���ٲ�ѯ
		}
		else {
			if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) { // ϵͳ����Ϊ����ϵͳʱ����������ѯ�ٶ�
				sllLastTimeTickCount = llTickCount + 60000; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
			}
			else {
				//m_pWebInquiry->SetCurrentInquiryTime(llTickCount - sllLastTimeTickCount);
				sllLastTimeTickCount = llTickCount;
			}
		}
	}
	return true;
}

bool CSinaRTDataSource::InquireRTData(const long) {
	if (!IsInquiring()) {
		const auto product = make_shared<CProductSinaRT>();
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}
