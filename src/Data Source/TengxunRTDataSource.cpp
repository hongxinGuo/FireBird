#include"pch.h"

#include"TengxunRTDataSource.h"
#include"ProductTengxunRT.h"

#include"ChinaMarket.h"

/// �ڲ��˸�����
CTengxunRTDataSourceImpPtr s_TengxunRTDataSourcePtr1 = nullptr;
CTengxunRTDataSourceImpPtr s_TengxunRTDataSourcePtr2 = nullptr;
CTengxunRTDataSourceImpPtr s_TengxunRTDataSourcePtr3 = nullptr;
CTengxunRTDataSourceImpPtr s_TengxunRTDataSourcePtr4 = nullptr;

CTengxunRTDataSource::CTengxunRTDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = _T("http://qt.gtimg.cn/q=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 900; // ��Ѷʵʱ���ݲ�ѯĬ��ֵ

	s_TengxunRTDataSourcePtr1 = make_shared<CTengxunRTDataSourceImp>();
	s_TengxunRTDataSourcePtr2 = make_shared<CTengxunRTDataSourceImp>();
	s_TengxunRTDataSourcePtr3 = make_shared<CTengxunRTDataSourceImp>();
	s_TengxunRTDataSourcePtr4 = make_shared<CTengxunRTDataSourceImp>();
	m_DataSourceContainer.at(0) = s_TengxunRTDataSourcePtr1;
	m_DataSourceContainer.at(1) = s_TengxunRTDataSourcePtr2;
	m_DataSourceContainer.at(2) = s_TengxunRTDataSourcePtr3;
	m_DataSourceContainer.at(3) = s_TengxunRTDataSourcePtr4;
}

/// <summary>
/// ����Ѷʵʱ������Ϊ��ѡ����Դ
/// ��Ѷ���ݾ��Ȳ������佻�������ľ���Ϊ�֣����ṩ�����Ϣ��
///
/// ��Ѷʵʱ���ݵ����紫���ٶ��޷��ﵽԤ�ڣ�<100ms�����ʶ�ʹ���ĸ����ݽ������沢��ִ�С�������Դ��ִ�о������ؽ�������ִֻ������ĵ��ȡ�
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
		// �ӵ��ó��п��е����湤���߳�
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
