#include"pch.h"

#include"TengxunRTDataSource.h"
#include"ProductTengxunRT.h"

#include"ChinaMarket.h"

CTengxunRTDataSource::CTengxunRTDataSource() {
	m_strInquiryFunction = _T("http://qt.gtimg.cn/q=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 900; // 腾讯实时数据查询默认值

	CTengxunRTDataSource::ConfigureSession();

	CTengxunRTDataSource::Reset();
}

bool CTengxunRTDataSource::Reset() {
	return true;
}

bool CTengxunRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const long long llTickCount = GetTickCount();

	if (gl_systemStatus.IsWebBusy()) return false; // 网络出现问题时，不申请腾讯实时数据。
	if (gl_pChinaMarket->IsSystemReady() && (llTickCount > (m_llLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime() * 5))) {
		if (IsWebError()) {
			m_llLastTimeTickCount = llTickCount + 10000; //网络出现错误时，延迟十秒再查询
		}
		else {
			if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) {
				m_llLastTimeTickCount = llTickCount + 60000; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
			}
			else {
				m_llLastTimeTickCount = llTickCount;
			}
		}
		if (!IsInquiring()) {
			InquireRTData(lCurrentTime);
			return true;
		}
	}
	return false;
}

bool CTengxunRTDataSource::InquireRTData(const long lCurrentTime) {
	if (!IsInquiring()) {
		const auto product = make_shared<CProductTengxunRT>();
		ASSERT(m_qProduct.empty());
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}

void CTengxunRTDataSource::GenerateCurrentInquiryMessage() {
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

void CTengxunRTDataSource::ConfigureSession() {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 4000); // 正常情况下Tengxun实时数据接收时间大致为300毫秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 4000); // 设置接收超时时间为4000毫秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // 设置发送超时时间为500毫秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当所有被查询的股票皆为非上市股票时，腾讯实时股票服务器会返回一个21个字符长的字符串：v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTengxunRTDataSource::IsInvalidTengxunRTData(CWebData& WebDataReceived) {
	char buffer[50]{};
	char* pBuffer = buffer;
	const CString strInvalidStock = _T("v_pv_none_match=\"1\";\n"); // 此为无效股票查询到的数据格式，共21个字符

	WebDataReceived.GetData(pBuffer, 21, WebDataReceived.GetCurrentPos());
	buffer[21] = 0x000;
	const CString str1 = buffer;

	if (str1.Compare(strInvalidStock) == 0) {
		ASSERT(WebDataReceived.GetBufferLength() == 21);
		return true;
	}
	else return false;
}
