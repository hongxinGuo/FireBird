#include"pch.h"
module;
module FireBird.DataSource.TengxunRT;

import FireBird.Product.TengxunRT;
import FireBird.Market.China;
import FireBird.System.Configuration;

CTengxunRTDataSource::CTengxunRTDataSource() {
	m_strInquiryFunction = _T("http://qt.gtimg.cn/q=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 900; // 腾讯实时数据查询默认值

	CTengxunRTDataSource::ConfigureInternetOption();

	CTengxunRTDataSource::Reset();
}

bool CTengxunRTDataSource::Reset() {
	return true;
}

bool CTengxunRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const long long llTickCount = GetTickCount();

	if (llTickCount > m_llLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()) {
		// 先判断下次申请时间。出现网络错误时无视之，继续下次申请。
		if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) {
			m_llLastTimeTickCount = llTickCount + 60000; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
		}
		else {
			if (!IsInquiring()) m_llLastTimeTickCount = llTickCount; // 只有当上一次申请结束后方调整计时基点，这样如果上一次申请超时结束后，保证尽快进行下一次申请。
		}
		if (!IsInquiring()) {
			const auto product = make_shared<CProductTengxunRT>();
			product->SetMarket(gl_pChinaMarket);
			ASSERT(!HaveInquiry());
			StoreInquiry(product);
			SetInquiring(true);
			return true;
		}
	}
	return false;
}

void CTengxunRTDataSource::CreateCurrentInquireString() {
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

void CTengxunRTDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 1000;
	m_internetOption.option_receive_timeout = 1000;
	m_internetOption.option_data_receive_timeout = 1000;
	m_internetOption.option_send_timeout = 200;
	m_internetOption.option_connect_retries = 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当所有被查询的股票皆为非上市股票时，腾讯实时股票服务器会返回一个21个字符长的字符串：v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTengxunRTDataSource::IsInvalidTengxunRTData(const CWebData& WebDataReceived) {
	const string_view sv = WebDataReceived.GetStringView(0, 21);

	if (sv.compare(_T("v_pv_none_match=\"1\";\n")) == 0) {
		ASSERT(WebDataReceived.GetBufferLength() == 21);
		return true;
	}
	else return false;
}
