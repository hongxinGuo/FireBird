#include"pch.h"

#include"NeteaseDaylineDataSource.h"
#include"ProductNeteaseDayline.h"

#include"ChinaMarket.h"

CNeteaseDaylineDataSource::CNeteaseDaylineDataSource() {
	m_pWebInquiry = nullptr;
	Reset();
}

CNeteaseDaylineDataSource::~CNeteaseDaylineDataSource() {
}

bool CNeteaseDaylineDataSource::Reset(void) {
	return true;
}

bool CNeteaseDaylineDataSource::UpdateStatus(void) {
	return true;
}

bool CNeteaseDaylineDataSource::Inquire(long lCurrentTime) {
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

bool CNeteaseDaylineDataSource::InquireRTData(long lCurrentTime) {
	if (!IsInquiring()) {
		CVirtualProductWebDataPtr product = make_shared<CProductNeteaseDayline>();
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}