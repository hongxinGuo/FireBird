#include"pch.h"

#include"NeteaseRTDataSource.h"
#include"ProductNeteaseRT.h"

#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"

CNeteaseRTDataSource::CNeteaseRTDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = _T("http://api.money.126.net/data/feed/");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 900; // 网易实时数据查询默认值

	CNeteaseRTDataSource::ConfigureInternetOption();
	CNeteaseRTDataSource::Reset();
}

bool CNeteaseRTDataSource::Reset() {
	return true;
}

bool CNeteaseRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const auto llTickCount = GetTickCount();
	if (llTickCount > m_PrevTimePoint + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()) {
		// 先判断下次申请时间。出现网络错误时无视之，继续下次申请。
		if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) { // 系统配置为：测试系统时，不降低轮询速度
			m_PrevTimePoint = llTickCount + 60000ms; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
		}
		else {
			if (!IsInquiring()) m_PrevTimePoint = llTickCount; // 只有当上一次申请结束后方调整计时基点，这样如果上一次申请超时结束后，保证尽快进行下一次申请。
		}
		// 后申请网络数据
		if (!IsInquiring()) {
			ASSERT(!HaveInquiry());
			Inquire(lCurrentTime);
			return true;
		}
	}
	return false;
}

void CNeteaseRTDataSource::Inquire(const long lCurrentTime) {
	if (!IsInquiring()) {
		ASSERT(!HaveInquiry());
		const CString strMessage = _T("http://api.money.126.net/data/feed/");
		const CString strStocks = gl_pChinaMarket->GetNeteaseStockInquiringMiddleStr(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock()); // 目前还是使用全部股票池
		const CString strNeteaseStockCode = strStocks.Left(7); //只提取第一个股票代码.网易代码格式为：0600000，100001，共七个字符
		gl_systemMessage.SetStockCodeForInquiringRTData(XferNeteaseToStandard(strNeteaseStockCode));
		const auto product = make_shared<CProductNeteaseRT>();
		product->SetMarket(gl_pChinaMarket);
		product->SetInquiryFunction(strMessage + strStocks);
		StoreInquiry(product);
		SetInquiring(true);
	}
}

void CNeteaseRTDataSource::CreateCurrentInquireString() {
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

/// <summary>
/// 网易实时数据的接收，不时出现错误数据。目前无法确定是服务器本身的错误还是接收中出现的错误。如果服务器本身就发送错了，就无法修正了。
/// 修改session各参数，试试能否解决问题。
/// </summary>
void CNeteaseRTDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 2000;
	m_internetOption.option_receive_timeout = 2000;
	m_internetOption.option_data_receive_timeout = 2000;
	m_internetOption.option_send_timeout = 500;
	m_internetOption.option_connect_retries = 1;
}
