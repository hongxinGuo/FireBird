#include"pch.h"

#include"SinaRTDataSourceImp.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"

/// <summary>
/// 新浪实时数据服务器要求提供报头验证数据。
///
/// </summary>
CSinaRTDataSourceImp::CSinaRTDataSourceImp() {
	// 2022年1月20日后，新浪实时数据服务器需要添加报头验证数据，格式为： Referer:https://finance.sina.com.cn
	// User-Agent部分只用于说明格式,即报头皆以\r\n（CRLF)结束
	//m_strHeaders = _T("User-Agent:FireBird\r\nReferer:https://finance.sina.com.cn\r\n");
	m_strHeaders = _T("Referer:https://finance.sina.com.cn\r\n");

	m_strInquiryFunction = _T("https://hq.sinajs.cn/list="); // 新浪实时数据服务器已使用https格式
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 850; // 新浪实时数据查询数量默认值

	CSinaRTDataSourceImp::ConfigureSession();

	CSinaRTDataSourceImp::Reset();
}

bool CSinaRTDataSourceImp::Reset() {
	return true;
}

bool CSinaRTDataSourceImp::GenerateInquiryMessage(const long lCurrentTime) {
	InquireRTData(lCurrentTime);
	return true;
}

bool CSinaRTDataSourceImp::InquireRTData(const long) {
	if (!IsInquiring()) {
		const auto product = make_shared<CProductSinaRT>();
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 新浪网络实时数据提取函数。
// 目前只提取前12000个股票的实时数据。
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSinaRTDataSourceImp::GenerateCurrentInquiryMessage() {
	ASSERT(m_pCurrentProduct != nullptr);
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

/// <summary>
/// 网易实时数据接收时，不时会出现解析失败的情况。感觉原因出在网易数据服务器发送错误，但还要继续观察。
/// 调整Session的选项可能会改善接收情况。
/// </summary>
void CSinaRTDataSourceImp::ConfigureSession() {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 3000); // 正常情况下sina实时数据接收时间不超过50毫秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 3000); // 设置接收超时时间为1000毫秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // 设置发送超时时间为500毫秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}
