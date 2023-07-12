#include"pch.h"

#include"SinaRTDataSource.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"

CSinaRTDataSourceImpPtr s_SinaRTDataSourcePtr1 = nullptr;
CSinaRTDataSourceImpPtr s_SinaRTDataSourcePtr2 = nullptr;
CSinaRTDataSourceImpPtr s_SinaRTDataSourcePtr3 = nullptr;
CSinaRTDataSourceImpPtr s_SinaRTDataSourcePtr4 = nullptr;
CSinaRTDataSourceImpPtr s_SinaRTDataSourcePtr5 = nullptr;
CSinaRTDataSourceImpPtr s_SinaRTDataSourcePtr6 = nullptr;
CSinaRTDataSourceImpPtr s_SinaRTDataSourcePtr7 = nullptr;
CSinaRTDataSourceImpPtr s_SinaRTDataSourcePtr8 = nullptr;

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

	s_SinaRTDataSourcePtr1 = make_shared<CSinaRTDataSourceImp>();
	s_SinaRTDataSourcePtr2 = make_shared<CSinaRTDataSourceImp>();
	s_SinaRTDataSourcePtr3 = make_shared<CSinaRTDataSourceImp>();
	s_SinaRTDataSourcePtr4 = make_shared<CSinaRTDataSourceImp>();
	s_SinaRTDataSourcePtr5 = make_shared<CSinaRTDataSourceImp>();
	s_SinaRTDataSourcePtr6 = make_shared<CSinaRTDataSourceImp>();
	s_SinaRTDataSourcePtr7 = make_shared<CSinaRTDataSourceImp>();
	s_SinaRTDataSourcePtr8 = make_shared<CSinaRTDataSourceImp>();
	m_DataSourceContainer.at(0) = s_SinaRTDataSourcePtr1;
	m_DataSourceContainer.at(1) = s_SinaRTDataSourcePtr2;
	m_DataSourceContainer.at(2) = s_SinaRTDataSourcePtr3;
	m_DataSourceContainer.at(3) = s_SinaRTDataSourcePtr4;
	m_DataSourceContainer.at(4) = s_SinaRTDataSourcePtr5;
	m_DataSourceContainer.at(5) = s_SinaRTDataSourcePtr6;
	m_DataSourceContainer.at(6) = s_SinaRTDataSourcePtr7;
	m_DataSourceContainer.at(7) = s_SinaRTDataSourcePtr8;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 新浪实时数据服务器的响应时间（20-80毫秒）足够快，但偶尔出现的网络延迟会达到（300-1500毫秒），且有时无法自动恢复正常情况。
// 故而使用八个数据接收器并行执行，以避开偶尔出现的网络颠簸。本数据源不执行具体下载解析任务，只执行具体任务的调度。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CSinaRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const long long llTickCount = GetTickCount();
	if ((llTickCount > (m_llLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()))) {
		if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) { // 系统配置为测试系统时，不降低轮询速度
			m_llLastTimeTickCount = llTickCount + 60000; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可
		}
		else {
			m_llLastTimeTickCount = llTickCount;
		}
		// 从池中调用实际执行工作线程
		for (const auto pDataSourceImp : m_DataSourceContainer) {
			if (!pDataSourceImp->IsInquiring() && !pDataSourceImp->IsWorkingThreadRunning()) {
				pDataSourceImp->Run(lCurrentTime);
				break;
			}
		}
		return true;
	}
	return false;
}
