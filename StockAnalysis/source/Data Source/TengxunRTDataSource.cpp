#include"pch.h"

#include"TengxunRTDataSource.h"
#include"ProductTengxunRT.h"

#include"ChinaMarket.h"

CTengxunRTDataSource::CTengxunRTDataSource() {
	Reset();
}

CTengxunRTDataSource::~CTengxunRTDataSource() {
}

bool CTengxunRTDataSource::Reset(void) {
	return true;
}

bool CTengxunRTDataSource::UpdateStatus(void) {
	return true;
}

void CTengxunRTDataSource::ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData) {
	pProductWebData->ParseAndStoreWebData(pWebData, this);
	SetInquiring(false); // 允许系统继续申请新的数据，随后再处理接收到的数据
}

bool CTengxunRTDataSource::Inquire(long lCurrentTime) {
	static long long sllLastTimeTickCount = 0;
	long long llTickCount = 0;

	llTickCount = GetTickCount64();
	if (gl_pChinaMarket->IsSystemReady() && llTickCount > (sllLastTimeTickCount + gl_systemConfigeration.GetChinaMarketRTDataInquiryTime() * 5)) {
		if (!IsInquiring()) {
			InquireRTData(lCurrentTime);
		}
		if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady()) {
			sllLastTimeTickCount = llTickCount + 60000; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
		}
		else {
			sllLastTimeTickCount = llTickCount;
		}
	}
	return true;
}

bool CTengxunRTDataSource::InquireRTData(long lCurrentTime) {
	if (!IsInquiring()) {
		CVirtualProductWebDataPtr product = make_shared<CProductTengxunRT>();
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}