#include"pch.h"

#include"TengxunRTDataSource.h"
#include"ProductTengxunRT.h"

#include"ChinaMarket.h"

CTengxunRTDataSourceImpPtr s_TengxunRTDataSourcePtr1 = nullptr;
CTengxunRTDataSourceImpPtr s_TengxunRTDataSourcePtr2 = nullptr;
CTengxunRTDataSourceImpPtr s_TengxunRTDataSourcePtr3 = nullptr;
CTengxunRTDataSourceImpPtr s_TengxunRTDataSourcePtr4 = nullptr;
CTengxunRTDataSourceImpPtr s_TengxunRTDataSourcePtr5 = nullptr;
CTengxunRTDataSourceImpPtr s_TengxunRTDataSourcePtr6 = nullptr;
CTengxunRTDataSourceImpPtr s_TengxunRTDataSourcePtr7 = nullptr;
CTengxunRTDataSourceImpPtr s_TengxunRTDataSourcePtr8 = nullptr;

CTengxunRTDataSource::CTengxunRTDataSource() {
	m_strInquiryFunction = _T("http://qt.gtimg.cn/q=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 900; // 腾讯实时数据查询默认值

	s_TengxunRTDataSourcePtr1 = make_shared<CTengxunRTDataSourceImp>();
	s_TengxunRTDataSourcePtr2 = make_shared<CTengxunRTDataSourceImp>();
	s_TengxunRTDataSourcePtr3 = make_shared<CTengxunRTDataSourceImp>();
	s_TengxunRTDataSourcePtr4 = make_shared<CTengxunRTDataSourceImp>();
	s_TengxunRTDataSourcePtr5 = make_shared<CTengxunRTDataSourceImp>();
	s_TengxunRTDataSourcePtr6 = make_shared<CTengxunRTDataSourceImp>();
	s_TengxunRTDataSourcePtr7 = make_shared<CTengxunRTDataSourceImp>();
	s_TengxunRTDataSourcePtr8 = make_shared<CTengxunRTDataSourceImp>();
	m_DataSourceContainer.at(0) = s_TengxunRTDataSourcePtr1;
	m_DataSourceContainer.at(1) = s_TengxunRTDataSourcePtr2;
	m_DataSourceContainer.at(2) = s_TengxunRTDataSourcePtr3;
	m_DataSourceContainer.at(3) = s_TengxunRTDataSourcePtr4;
	m_DataSourceContainer.at(4) = s_TengxunRTDataSourcePtr5;
	m_DataSourceContainer.at(5) = s_TengxunRTDataSourcePtr6;
	m_DataSourceContainer.at(6) = s_TengxunRTDataSourcePtr7;
	m_DataSourceContainer.at(7) = s_TengxunRTDataSourcePtr8;
}

/// <summary>
/// 将腾讯实时数据列为备选数据源
/// 腾讯数据精度不够，其交易数量的精度为手，不提供零股信息。
///
/// 腾讯实时数据的网络传输速度无法达到预期（<100ms），故而使用八个数据接收器并行执行。本数据源不执行具体下载解析任务，只执行具体任务的调度。
///
/// </summary>
/// <param name="lCurrentTime"></param>
/// <returns></returns>
bool CTengxunRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const long long llTickCount = GetTickCount();

	if (llTickCount > m_llLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()) {
		if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) {
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
