#include"pch.h"

#include"SinaRTDataSource.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"

CSinaRTDataSource::CSinaRTDataSource() {
	m_pWebInquiry = nullptr;
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

bool CSinaRTDataSource::InquireRTData(long lCurrentTime) {
	if (!IsInquiring()) {
		CVirtualProductWebDataPtr product = make_shared<CProductSinaRT>();
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}