#include"pch.h"

#include"SinaRTDataSource.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"

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

	CSinaRTDataSource::ConfigureSession();

	CSinaRTDataSource::Reset();
}

bool CSinaRTDataSource::Reset() {
	return true;
}

bool CSinaRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const long long llTickCount = GetTickCount();
	if (llTickCount > m_llLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()) {
		// 先判断下次申请时间。出现网络错误时无视之，继续下次申请。
		if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) { // 系统配置为测试系统时，不降低轮询速度
			m_llLastTimeTickCount = llTickCount + 60000; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
		}
		else {
			if (!IsInquiring()) m_llLastTimeTickCount = llTickCount; // 只有当上一次申请结束后方调整计时基点，这样如果上一次申请超时结束后，保证尽快进行下一次申请。
		}
		// 后申请网络数据
		if (!IsInquiring()) {
			const auto product = make_shared<CProductSinaRT>();
			product->SetMarket(gl_pChinaMarket);
			StoreInquiry(product);
			SetInquiring(true);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 新浪网络实时数据提取函数。
// 目前只提取前12000个股票的实时数据。
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSinaRTDataSource::GenerateCurrentInquiryMessage() {
	ASSERT(m_pCurrentProduct != nullptr);
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

/// <summary>
/// 
/// 调整Session的选项可能会改善接收情况。
///
/// </summary>
void CSinaRTDataSource::ConfigureSession() {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000); // 正常情况下sina实时数据接收时间不超过50毫秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 750); // 设置接收超时时间为500毫秒
	m_pSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 750); // 设置接收超时时间为500毫秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // 设置发送超时时间为500毫秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}
