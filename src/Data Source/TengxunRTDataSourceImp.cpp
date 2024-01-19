#include"pch.h"

#include"TengxunRTDataSourceImp.h"
#include"ProductTengxunRT.h"

#include "ChinaMarket.h"

CTengxunRTDataSourceImp::CTengxunRTDataSourceImp() {
	m_strInquiryFunction = _T("http://qt.gtimg.cn/q=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 900; // 腾讯实时数据查询默认值

	CTengxunRTDataSourceImp::ConfigureSession();

	CTengxunRTDataSourceImp::Reset();
}

bool CTengxunRTDataSourceImp::Reset() { return true; }

bool CTengxunRTDataSourceImp::GenerateInquiryMessage(const long lCurrentTime) {
	if (!IsInquiring()) {
		const auto product = make_shared<CProductTengxunRT>();
		product->SetMarket(gl_pChinaMarket);
		ASSERT(m_qProduct.empty());
		StoreInquiry(product);
		SetInquiring(true);
	}
	return true;
}

void CTengxunRTDataSourceImp::GenerateCurrentInquiryMessage() { m_strInquiry = m_pCurrentProduct->CreateMessage(); }

void CTengxunRTDataSourceImp::ConfigureSession() {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000); // 正常情况下Tengxun实时数据接收时间大致为300毫秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 1000); // 设置接收超时时间为1000毫秒
	m_pSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 1000); // 设置接收超时时间为1秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 200); // 设置发送超时时间为200毫秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当所有被查询的股票皆为非上市股票时，腾讯实时股票服务器会返回一个21个字符长的字符串：v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTengxunRTDataSourceImp::IsInvalidTengxunRTData(const CWebData& WebDataReceived) {
	const string_view sv = WebDataReceived.GetStringView(0, 21);

	if (sv.compare(_T("v_pv_none_match=\"1\";\n")) == 0) {
		ASSERT(WebDataReceived.GetBufferLength() == 21);
		return true;
	}
	else return false;
}
