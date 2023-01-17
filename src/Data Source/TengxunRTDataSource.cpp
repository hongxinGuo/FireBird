#include"pch.h"

#include"TengxunRTDataSource.h"
#include"ProductTengxunRT.h"

#include"ChinaMarket.h"

CTengxunRTDataSource::CTengxunRTDataSource() { Reset(); }

bool CTengxunRTDataSource::Reset(void) { return true; }

bool CTengxunRTDataSource::UpdateStatus(void) { return true; }

bool CTengxunRTDataSource::Inquire(const long lCurrentTime) {
	const long long llTickCount = GetTickCount64();
	static long long sllLastTimeTickCount = 0;

	if (gl_systemStatus.IsWebBusy()) return false; // �����������ʱ����������Ѷʵʱ���ݡ�
	if (gl_pChinaMarket->IsSystemReady() && (llTickCount > (sllLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime() * 5))) {
		if (m_pWebInquiry->IsWebError()) {
			sllLastTimeTickCount = llTickCount + 10000; //������ִ���ʱ���ӳ�ʮ���ٲ�ѯ
		}
		else {
			if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady()) {
				sllLastTimeTickCount = llTickCount + 60000; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
			}
			else {
				sllLastTimeTickCount = llTickCount;
			}
		}
		if (!IsInquiring()) {
			InquireRTData(lCurrentTime);
		}
	}
	return true;
}

bool CTengxunRTDataSource::InquireRTData(const long lCurrentTime) {
	if (!IsInquiring()) {
		const auto product = make_shared<CProductTengxunRT>();
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}
