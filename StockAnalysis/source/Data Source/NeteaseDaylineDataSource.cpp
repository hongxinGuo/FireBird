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

// 抓取日线数据.开始于11:45:01
// 最多使用四个引擎，否则容易被网易服务器拒绝服务。一般还是用两个为好。
bool CNeteaseDaylineDataSource::Inquire(long lCurrentTime) {
	if (gl_pChinaMarket->IsSystemReady() && gl_pChinaMarket->IsDayLineNeedUpdate() && gl_pChinaMarket->IsDummyTime() && (gl_pChinaMarket->GetMarketTime() > 114500)) {
		if (!IsInquiring()) {
			CVirtualProductWebDataPtr product = make_shared<CProductNeteaseDayline>();
			StoreInquiry(product);
			SetInquiring(true);
			return true;
		}
	}
	return true;
}