#include"pch.h"

#include"NeteaseRTDataSource.h"
#include"ProductNeteaseRT.h"

#include"ChinaMarket.h"

CNeteaseRTDataSource::CNeteaseRTDataSource() {
	m_pWebInquiry = nullptr;
	m_fPermitToConcurrentProceed = true;// 网易实时数据源采用并行处理模式，在解析数据之前就允许再次申请数据，以保持数据的申请速度。
	Reset();
}

CNeteaseRTDataSource::~CNeteaseRTDataSource() {
}

bool CNeteaseRTDataSource::Reset(void) {
	return true;
}

bool CNeteaseRTDataSource::UpdateStatus(void) {
	return true;
}

bool CNeteaseRTDataSource::Inquire(long lCurrentTime) {
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

bool CNeteaseRTDataSource::InquireRTData(long lCurrentTime) {
	if (!IsInquiring()) {
		CVirtualProductWebDataPtr product = make_shared<CProductNeteaseRT>();
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}