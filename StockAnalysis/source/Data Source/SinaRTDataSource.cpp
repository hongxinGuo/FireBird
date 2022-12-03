#include"pch.h"

#include"SinaRTDataSource.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"

CSinaRTDataSource::CSinaRTDataSource() {
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 新浪实时数据先允许接收下次的数据，然后再开始处理本次数据。这样优先保证查询的速度。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSinaRTDataSource::ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData) {
	SetInquiring(false); // 允许系统继续申请新的数据，随后再处理接收到的数据
	pProductWebData->ParseAndStoreWebData(pWebData);
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
			if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady()) {
				if (!gl_systemConfigeration.IsDebugMode()) { // 系统配置为：测试系统时，不降低轮询速度
					sllLastTimeTickCount = llTickCount + 60000; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
				}
			}
			else {
				sllLastTimeTickCount = llTickCount;
			}
		}
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