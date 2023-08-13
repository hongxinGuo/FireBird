#include"pch.h"

#include"SinaRTDataSource.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"

CSinaRTDataSourceImpPtr s_SinaRTDataSourcePtr1 = nullptr;
CSinaRTDataSourceImpPtr s_SinaRTDataSourcePtr2 = nullptr;
CSinaRTDataSourceImpPtr s_SinaRTDataSourcePtr3 = nullptr;
CSinaRTDataSourceImpPtr s_SinaRTDataSourcePtr4 = nullptr;

/// <summary>
/// ����ʵʱ���ݷ�����Ҫ���ṩ��ͷ��֤���ݡ�
///
/// </summary>
CSinaRTDataSource::CSinaRTDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	// 2022��1��20�պ�����ʵʱ���ݷ�������Ҫ��ӱ�ͷ��֤���ݣ���ʽΪ�� Referer:https://finance.sina.com.cn
	// User-Agent����ֻ����˵����ʽ,����ͷ����\r\n��CRLF)����
	//m_strHeaders = _T("User-Agent:FireBird\r\nReferer:https://finance.sina.com.cn\r\n");
	m_strHeaders = _T("Referer:https://finance.sina.com.cn\r\n");

	m_strInquiryFunction = _T("https://hq.sinajs.cn/list="); // ����ʵʱ���ݷ�������ʹ��https��ʽ
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 850; // ����ʵʱ���ݲ�ѯ����Ĭ��ֵ

	s_SinaRTDataSourcePtr1 = make_shared<CSinaRTDataSourceImp>();
	s_SinaRTDataSourcePtr2 = make_shared<CSinaRTDataSourceImp>();
	s_SinaRTDataSourcePtr3 = make_shared<CSinaRTDataSourceImp>();
	s_SinaRTDataSourcePtr4 = make_shared<CSinaRTDataSourceImp>();
	m_DataSourceContainer.at(0) = s_SinaRTDataSourcePtr1;
	m_DataSourceContainer.at(1) = s_SinaRTDataSourcePtr2;
	m_DataSourceContainer.at(2) = s_SinaRTDataSourcePtr3;
	m_DataSourceContainer.at(3) = s_SinaRTDataSourcePtr4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ���ݷ���������Ӧʱ�䣨20-80���룩�㹻�죬��ż�����ֵ������ӳٻ�ﵽ300-1500���룬����ʱ�޷��Զ��ָ����������
// �ʶ�ʹ���ĸ����ݽ���������ִ�У��Աܿ�ż�����ֵ����������
//
// ������Դ��ִ�о������ؽ�������ִֻ������ĵ��ȡ�
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CSinaRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const long long llTickCount = GetTickCount();
	if ((llTickCount > (m_llLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()))) {
		if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) { // ϵͳ����Ϊ����ϵͳʱ����������ѯ�ٶ�
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
