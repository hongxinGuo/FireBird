#include"pch.h"

#include"SinaRTDataSource.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"

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

bool CSinaRTDataSource::Reset(void) {
	return true;
}

bool CSinaRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const long long llTickCount = GetTickCount64();
	if (static long long sllLastTimeTickCount = 0; llTickCount > (sllLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime())) {
		// 先判断下次的申请时间。因网络错误只在顺利接收网络数据后方才重置。
		if (IsWebError()) {
			sllLastTimeTickCount = llTickCount + 10000; //网络出现错误时，延迟十秒再查询
		}
		else {
			if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) { // 系统配置为测试系统时，不降低轮询速度
				sllLastTimeTickCount = llTickCount + 60000; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
			}
			else {
				//m_pWebInquiry->SetCurrentInquiryTime(llTickCount - sllLastTimeTickCount);
				sllLastTimeTickCount = llTickCount;
			}
		}
		// 后申请网络数据
		if (!IsInquiring()) {
			InquireRTData(lCurrentTime);
		}
	}
	return true;
}

bool CSinaRTDataSource::InquireRTData(const long) {
	if (!IsInquiring()) {
		const auto product = make_shared<CProductSinaRT>();
		const CString strMessage = _T("https://hq.sinajs.cn/list=");
		// 申请下一批次股票实时数据
		// 如果处于寻找今日活跃股票期间（9:10--9:29, 11:31--12:59),则使用全局股票池
		// 开市时使用今日活跃股票池
		const CString strStocks = gl_pChinaMarket->GetSinaStockInquiringStr(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock());
		const CString strSinaStockCode = strStocks.Left(8); // 只提取第一个股票代码。新浪代码格式为：sh000001，共八个字符。
		gl_systemMessage.SetStockCodeForInquiringRTData(XferSinaToStandard(strSinaStockCode));
		product->SetInquiryFunction(strMessage + strStocks);
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
void CSinaRTDataSource::CreateInquiryMessageFromCurrentProduct(void) {
	ASSERT(m_pCurrentProduct != nullptr);
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

/// <summary>
/// 网易实时数据接收时，不时会出现解析失败的情况。感觉原因出在网易数据服务器发送错误，但还要继续观察。
/// 调整Session的选项可能会改善接收情况。
/// </summary>
/// <param name=""></param>
void CSinaRTDataSource::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 3000); // 正常情况下sina实时数据接收时间不超过50毫秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 3000); // 设置接收超时时间为4000毫秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // 设置发送超时时间为500毫秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}
