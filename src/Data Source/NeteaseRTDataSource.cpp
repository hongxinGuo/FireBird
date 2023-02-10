#include"pch.h"

#include"NeteaseRTDataSource.h"
#include"ProductNeteaseRT.h"

#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"

CNeteaseRTDataSource::CNeteaseRTDataSource() {
	m_strInquiryFunction = _T("http://api.money.126.net/data/feed/");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 900; // 网易实时数据查询默认值

	CNeteaseRTDataSource::ConfigureSession();
	CNeteaseRTDataSource::Reset();
}

bool CNeteaseRTDataSource::Reset(void) {
	return true;
}

bool CNeteaseRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const ULONGLONG llTickCount = GetTickCount64();
	if (static ULONGLONG sllLastTimeTickCount = 0; llTickCount > (sllLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime())) {
		// 先判断下次的申请时间。因网络错误只在顺利接收网络数据后方才重置。
		if (IsWebError()) {
			sllLastTimeTickCount = llTickCount + 10000; //网络出现错误时，延迟十秒再查询
		}
		else {
			if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) { // 系统配置为：测试系统时，不降低轮询速度
				sllLastTimeTickCount = llTickCount + 60000; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
			}
			else {
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

bool CNeteaseRTDataSource::InquireRTData(const long lCurrentTime) {
	if (!IsInquiring()) {
		const auto product = make_shared<CProductNeteaseRT>();
		const CString strMessage = _T("http://api.money.126.net/data/feed/");
		const CString strStocks = gl_pChinaMarket->GetNeteaseStockInquiringMiddleStr(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock()); // 目前还是使用全部股票池
		const CString strNeteaseStockCode = strStocks.Left(7); //只提取第一个股票代码.网易代码格式为：0600000，100001，共七个字符
		gl_systemMessage.SetStockCodeForInquiringRTData(XferNeteaseToStandard(strNeteaseStockCode));
		product->SetInquiryFunction(strMessage + strStocks);
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 目前neteaseRTData使用nlohmann json库函数解析，其解析速度大致是property tree的两倍。
// 解析900个网易数据，DEBUG模式下大致时间要300-400毫秒；Release模式下也就50毫秒。
//
// 在此解析数据的话，系统容易锁死，原因大概与网络读取数据有关，但确切原因不明，故而决定将解析功能移至DataSource的工作线程中。
// 且在使用全局优化来编译JsonParse.cpp文件后，其速度在DEBUG模式下也能够达到要求，故而无需特别处理了。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNeteaseRTDataSource::ParseData(CWebDataPtr pWebData) {
	return true;
}

void CNeteaseRTDataSource::CreateInquiryMessageFromCurrentProduct(void) {
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

/// <summary>
/// 网易实时数据的接收，不时出现错误数据。目前无法确定是服务器本身的错误还是接收中出现的错误。如果服务器本身就发送错了，就无法修正了。
/// 修改session各参数，试试能否解决问题。
/// </summary>
void CNeteaseRTDataSource::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 3000); // 设置连接超时时间为4000毫秒。 正常情况下网易实时数据接收时间不超过200毫秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 3000); // 设置接收超时时间为4000毫秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 200); // 设置发送超时时间为500毫秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 重试一次。
}
