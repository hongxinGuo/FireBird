#include"pch.h"

#include"TengxunRTDataSource.h"
#include"ProductTengxunRT.h"

#include"ChinaMarket.h"

CTengxunRTDataSource::CTengxunRTDataSource() { Reset(); }

bool CTengxunRTDataSource::Reset(void) { return true; }

bool CTengxunRTDataSource::UpdateStatus(void) { return true; }

bool CTengxunRTDataSource::Inquire(const long lCurrentTime) {
	const long long llTickCount = GetTickCount64();
	if (static long long sllLastTimeTickCount = 0; gl_pChinaMarket->IsSystemReady() && llTickCount > (sllLastTimeTickCount + gl_systemConfiguration.
		GetChinaMarketRTDataInquiryTime() * 5)) {
		if (!IsInquiring()) { InquireRTData(lCurrentTime); }
		if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady()) {
			sllLastTimeTickCount = llTickCount + 60000; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
		}
		else { sllLastTimeTickCount = llTickCount; }
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
