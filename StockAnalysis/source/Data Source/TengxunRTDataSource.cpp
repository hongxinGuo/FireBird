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

bool CTengxunRTDataSource::Inquire(long lCurrentTime) {
	static long long sllLastTimeTickCount = 0;

	if (m_pWebInquiry->IsWebError()) {
		m_pWebInquiry->SetWebError(false);
	}
	if (gl_pChinaMarket->GetCurrentTickCount() > (sllLastTimeTickCount + gl_systemConfigeration.GetChinaMarketRTDataInquiryTime())) {
		if (!IsInquiring()) {
			InquireRTData(lCurrentTime);
		}
		if (IsInquiring())	sllLastTimeTickCount = gl_pChinaMarket->GetCurrentTickCount();
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