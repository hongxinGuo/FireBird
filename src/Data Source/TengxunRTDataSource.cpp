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
	m_lInquiringNumber = 900; // ��Ѷʵʱ���ݲ�ѯĬ��ֵ

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
/// ����Ѷʵʱ������Ϊ��ѡ����Դ
/// ��Ѷ���ݾ��Ȳ������佻�������ľ���Ϊ�֣����ṩ�����Ϣ��
///
/// ��Ѷʵʱ���ݵ����紫���ٶ��޷��ﵽԤ�ڣ�<100ms�����ʶ�ʹ�ð˸����ݽ���������ִ�С�������Դ��ִ�о������ؽ�������ִֻ�о�������ĵ��ȡ�
///
/// </summary>
/// <param name="lCurrentTime"></param>
/// <returns></returns>
bool CTengxunRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const long long llTickCount = GetTickCount();

	if (llTickCount > m_llLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()) {
		if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) {
			m_llLastTimeTickCount = llTickCount + 60000; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
		}
		else {
			m_llLastTimeTickCount = llTickCount;
		}
		// �ӳ��е���ʵ��ִ�й����߳�
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
