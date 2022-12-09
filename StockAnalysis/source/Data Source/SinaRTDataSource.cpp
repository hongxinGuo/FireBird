#include"pch.h"

#include"SinaRTDataSource.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"

CSinaRTDataSource::CSinaRTDataSource() {
	Reset();
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
			if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfigeration.
				IsDebugMode()) {
				// 系统配置为：测试系统时，不降低轮询速度
				sllLastTimeTickCount = llTickCount + 60000; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
			}
			else {
				//m_pWebInquiry->SetCurrentInquiryTime(llTickCount - sllLastTimeTickCount);

				sllLastTimeTickCount = llTickCount;
			}
		}
	}
	return true;
}

bool CSinaRTDataSource::InquireRTData(long lCurrentTime) {
	if (!IsInquiring()) {
		const auto product = make_shared<CProductSinaRT>();
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}
