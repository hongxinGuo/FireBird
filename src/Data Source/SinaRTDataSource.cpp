#include"pch.h"

#include"SinaRTDataSource.h"
//#include"SinaRTWebInquiry.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"

CSinaRTDataSource::CSinaRTDataSource() { Reset(); }

bool CSinaRTDataSource::Reset(void) { return true; }

bool CSinaRTDataSource::UpdateStatus(void) { return true; }

bool CSinaRTDataSource::Inquire(const long lCurrentTime) {
	const long long llTickCount = GetTickCount64();
	if (static long long sllLastTimeTickCount = 0; llTickCount > (sllLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime())) {
		// 先判断下次的申请时间。因网络错误只在顺利接收网络数据后方才重置。
		if (m_pWebInquiry->IsWebError()) {
			sllLastTimeTickCount = llTickCount + 10000; //网络出现错误时，延迟十秒再查询
		}
		else {
			if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) { // 系统配置为测试系统时，不降低轮询速度
				sllLastTimeTickCount = llTickCount + 60000; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
			}
			else {
				//m_pWebInquiry->SetCurrentInquiryTime(llTickCount - sllLastTimeTickCount);
				sllLastTimeTickCount = llTickCount;
			}
		}
		// 后申请网络数据
		if (!IsInquiring()) {
			InquireRTData(lCurrentTime);
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
