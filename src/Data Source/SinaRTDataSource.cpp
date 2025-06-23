#include"pch.h"

#include"SinaRTDataSource.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"
#include "WebData.h"

/// <summary>
/// 新浪实时数据服务器要求提供报头验证数据。
///
/// </summary>
CSinaRTDataSource::CSinaRTDataSource() {
	// 2022年1月20日后，新浪实时数据服务器需要添加报头验证数据，格式为： Referer:https://finance.sina.com.cn
	// User-Agent部分只用于说明格式,即报头皆以\r\n（CRLF)结束
	//m_strHeaders = _T("User-Agent:FireBird\r\nReferer:https://finance.sina.com.cn\r\n");
	m_strHeaders = _T("Referer:https://finance.sina.com.cn\r\n");
	m_strInquiryFunction = _T("https://hq.sinajs.cn/list="); // 新浪实时数据服务器已使用https格式
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 850; // 新浪实时数据查询数量默认值

	CSinaRTDataSource::ConfigureInternetOption();

	CSinaRTDataSource::Reset();
}

bool CSinaRTDataSource::Reset() {
	return true;
}

bool CSinaRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const auto llTickCount = GetTickCount();
	if (llTickCount < m_PrevInquireTimePoint + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()) return false;
	// 先判断下次申请时间。出现网络错误时无视之，继续下次申请。
	if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) { // 系统配置为测试系统时，不降低轮询速度
		m_PrevInquireTimePoint = llTickCount + 60000ms; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
	}
	else if (GetCurrentInquiryTime() > 2000) {
		SetCurrentInquiryTime(500);
		m_PrevInquireTimePoint = llTickCount + 3000ms; // 如果响应时间超过1000ms，则等待3秒后再申请。
		return false;
	}
	else {
		if (!IsInquiring()) {
			m_PrevInquireTimePoint = llTickCount; // 只有当上一次申请结束后方调整计时基点，这样如果上一次申请超时结束后，保证尽快进行下一次申请。
		}
	}
	// 后申请网络数据
	if (!IsInquiring()) {
		const auto product = make_shared<CProductSinaRT>();
		product->SetMarket(gl_pChinaMarket);
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
void CSinaRTDataSource::CreateCurrentInquireString() {
	ASSERT(m_pCurrentProduct != nullptr);
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 当申请新浪数据时未添加报头验证数据（格式为： Referer:https://finance.sina.com.cn），系统返回数据为："Forbidden"。
///
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum_ErrorMessageData CSinaRTDataSource::IsAErrorMessageData(const CWebDataPtr& pWebData) {
	m_eErrorMessageData = ERROR_NO_ERROR__;
	if (m_dwHTTPStatusCode == 200) return m_eErrorMessageData; // OK? return no error

	if (pWebData->GetBufferLength() == 9) { // 是字符串"Forbidden"？
		const string_view s = pWebData->GetStringView(0, 9);
		if (s == _T("Forbidden")) {
			m_eErrorMessageData = ERROR_SINA_HEADER_NEEDED__;
		}
	}
	switch (m_eErrorMessageData) {
	case ERROR_SINA_HEADER_NEEDED__:
		ReportErrorNotHandled(_T("inquiry headed needed"));
		break;
	case ERROR_NO_ERROR__:
		break;
	default:
		ASSERT(0);
		break;
	}
	return m_eErrorMessageData;;
}

/// <summary>
/// 
/// 调整Session的选项可能会改善接收情况。
///
/// </summary>
void CSinaRTDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 1000;
	m_internetOption.option_receive_timeout = 1000;
	m_internetOption.option_data_receive_timeout = 1000;
	m_internetOption.option_send_timeout = 1000;
	m_internetOption.option_connect_retries = 1;
}
