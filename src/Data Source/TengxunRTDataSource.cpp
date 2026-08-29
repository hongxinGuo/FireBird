#include"pch.h"

#include"TengxunRTDataSource.h"
#include"ProductTengxunRT.h"

#include "ChinaMarket.h"
#include "SystemConfiguration.h"

using namespace std;

CTengxunRTDataSource::CTengxunRTDataSource() {
	m_strInquiryFunction = "http://qt.gtimg.cn/q=";
	m_token = "";
	m_lInquiringNumber = 900; // 腾讯实时数据查询默认值

	CTengxunRTDataSource::ConfigureInternetOption();

	CTengxunRTDataSource::Reset();
}

bool CTengxunRTDataSource::Reset() {
	return true;
}

bool CTengxunRTDataSource::GenerateInquiryMessage(const local_seconds& currentTime) {
	const auto llTickCount = GetTickCount();

	if (llTickCount > m_PrevInquireTimePoint + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()) {
		// 先判断下次申请时间。出现网络错误时无视之，继续下次申请。
		if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) {
			m_PrevInquireTimePoint = llTickCount + 3000ms; // 完全轮询一遍后，非交易时段3秒左右更新一次即可
		}
		else {
			if (!IsInquiring()) m_PrevInquireTimePoint = llTickCount; // 只有当上一次申请结束后方调整计时基点，这样如果上一次申请超时结束后，保证尽快进行下一次申请。
		}
		if (!IsInquiring()) {
			const auto product = make_shared<CProductTengxunRT>();
			ABSL_DCHECK(!HaveInquiry());
			StoreInquiry(product);
			return true;
		}
	}
	return false;
}

void CTengxunRTDataSource::CreateCurrentInquireString() {
	m_pInquiryStrings = m_pCurrentProduct->CreateMessage();
}

void CTengxunRTDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 1000;
	m_internetOption.option_receive_timeout = 1000;
	m_internetOption.option_data_receive_timeout = 1000;
	m_internetOption.option_send_timeout = 200;
	m_internetOption.option_connect_retries = 1;
}
