#include"pch.h"

#include"SinaRTDataSource.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"
#include "WebData.h"

/// <summary>
/// ����ʵʱ���ݷ�����Ҫ���ṩ��ͷ��֤���ݡ�
///
/// </summary>
CSinaRTDataSource::CSinaRTDataSource() {
	// 2022��1��20�պ�����ʵʱ���ݷ�������Ҫ��ӱ�ͷ��֤���ݣ���ʽΪ�� Referer:https://finance.sina.com.cn
	// User-Agent����ֻ����˵����ʽ,����ͷ����\r\n��CRLF)����
	//m_strHeaders = _T("User-Agent:FireBird\r\nReferer:https://finance.sina.com.cn\r\n");
	m_strHeaders = _T("Referer:https://finance.sina.com.cn\r\n");
	m_strInquiryFunction = _T("https://hq.sinajs.cn/list="); // ����ʵʱ���ݷ�������ʹ��https��ʽ
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 850; // ����ʵʱ���ݲ�ѯ����Ĭ��ֵ

	CSinaRTDataSource::ConfigureInternetOption();

	CSinaRTDataSource::Reset();
}

bool CSinaRTDataSource::Reset() {
	return true;
}

bool CSinaRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const auto llTickCount = GetTickCount();
	if (llTickCount < m_PrevInquireTimePoint + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()) return false;
	// ���ж��´�����ʱ�䡣�����������ʱ����֮�������´����롣
	if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) { // ϵͳ����Ϊ����ϵͳʱ����������ѯ�ٶ�
		m_PrevInquireTimePoint = llTickCount + 60000ms; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
	}
	else if (GetCurrentInquiryTime() > 2000) {
		SetCurrentInquiryTime(500);
		m_PrevInquireTimePoint = llTickCount + 3000ms; // �����Ӧʱ�䳬��1000ms����ȴ�3��������롣
		return false;
	}
	else {
		if (!IsInquiring()) {
			m_PrevInquireTimePoint = llTickCount; // ֻ�е���һ����������󷽵�����ʱ���㣬���������һ�����볬ʱ�����󣬱�֤���������һ�����롣
		}
	}
	// ��������������
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
// ��������ʵʱ������ȡ������
// Ŀǰֻ��ȡǰ12000����Ʊ��ʵʱ���ݡ�
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSinaRTDataSource::CreateCurrentInquireString() {
	ASSERT(m_pCurrentProduct != nullptr);
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// ��������������ʱδ��ӱ�ͷ��֤���ݣ���ʽΪ�� Referer:https://finance.sina.com.cn����ϵͳ��������Ϊ��"Forbidden"��
///
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum_ErrorMessageData CSinaRTDataSource::IsAErrorMessageData(const CWebDataPtr& pWebData) {
	m_eErrorMessageData = ERROR_NO_ERROR__;
	if (m_dwHTTPStatusCode == 200) return m_eErrorMessageData; // OK? return no error

	if (pWebData->GetBufferLength() == 9) { // ���ַ���"Forbidden"��
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
/// ����Session��ѡ����ܻ���ƽ��������
///
/// </summary>
void CSinaRTDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 1000;
	m_internetOption.option_receive_timeout = 1000;
	m_internetOption.option_data_receive_timeout = 1000;
	m_internetOption.option_send_timeout = 1000;
	m_internetOption.option_connect_retries = 1;
}
