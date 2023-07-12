#include"pch.h"

#include"SinaRTDataSourceImp.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"

/// <summary>
/// ����ʵʱ���ݷ�����Ҫ���ṩ��ͷ��֤���ݡ�
///
/// </summary>
CSinaRTDataSourceImp::CSinaRTDataSourceImp() {
	// 2022��1��20�պ�����ʵʱ���ݷ�������Ҫ��ӱ�ͷ��֤���ݣ���ʽΪ�� Referer:https://finance.sina.com.cn
	// User-Agent����ֻ����˵����ʽ,����ͷ����\r\n��CRLF)����
	//m_strHeaders = _T("User-Agent:FireBird\r\nReferer:https://finance.sina.com.cn\r\n");
	m_strHeaders = _T("Referer:https://finance.sina.com.cn\r\n");

	m_strInquiryFunction = _T("https://hq.sinajs.cn/list="); // ����ʵʱ���ݷ�������ʹ��https��ʽ
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 850; // ����ʵʱ���ݲ�ѯ����Ĭ��ֵ

	CSinaRTDataSourceImp::ConfigureSession();

	CSinaRTDataSourceImp::Reset();
}

bool CSinaRTDataSourceImp::Reset() {
	return true;
}

bool CSinaRTDataSourceImp::GenerateInquiryMessage(const long lCurrentTime) {
	InquireRTData(lCurrentTime);
	return true;
}

bool CSinaRTDataSourceImp::InquireRTData(const long) {
	if (!IsInquiring()) {
		const auto product = make_shared<CProductSinaRT>();
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
void CSinaRTDataSourceImp::GenerateCurrentInquiryMessage() {
	ASSERT(m_pCurrentProduct != nullptr);
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

/// <summary>
/// ����ʵʱ���ݽ���ʱ����ʱ����ֽ���ʧ�ܵ�������о�ԭ������������ݷ��������ʹ��󣬵���Ҫ�����۲졣
/// ����Session��ѡ����ܻ���ƽ��������
/// </summary>
void CSinaRTDataSourceImp::ConfigureSession() {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 3000); // ���������sinaʵʱ���ݽ���ʱ�䲻����50���롣
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 3000); // ���ý��ճ�ʱʱ��Ϊ1000����
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // ���÷��ͳ�ʱʱ��Ϊ500����
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1������
}
