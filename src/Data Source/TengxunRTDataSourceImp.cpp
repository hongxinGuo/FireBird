#include"pch.h"

#include"TengxunRTDataSourceImp.h"
#include"ProductTengxunRT.h"

CTengxunRTDataSourceImp::CTengxunRTDataSourceImp() {
	m_strInquiryFunction = _T("http://qt.gtimg.cn/q=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 900; // ��Ѷʵʱ���ݲ�ѯĬ��ֵ

	CTengxunRTDataSourceImp::ConfigureSession();

	CTengxunRTDataSourceImp::Reset();
}

bool CTengxunRTDataSourceImp::Reset() {
	return true;
}

bool CTengxunRTDataSourceImp::GenerateInquiryMessage(const long lCurrentTime) {
	InquireRTData(lCurrentTime);
	return true;
}

void CTengxunRTDataSourceImp::InquireRTData(const long lCurrentTime) {
	if (!IsInquiring()) {
		const auto product = make_shared<CProductTengxunRT>();
		ASSERT(m_qProduct.empty());
		StoreInquiry(product);
		SetInquiring(true);
	}
}

void CTengxunRTDataSourceImp::GenerateCurrentInquiryMessage() {
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

void CTengxunRTDataSourceImp::ConfigureSession() {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 2000); // ���������Tengxunʵʱ���ݽ���ʱ�����Ϊ300���롣
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 2000); // ���ý��ճ�ʱʱ��Ϊ1000����
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // ���÷��ͳ�ʱʱ��Ϊ200����
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1������
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����б���ѯ�Ĺ�Ʊ��Ϊ�����й�Ʊʱ����Ѷʵʱ��Ʊ�������᷵��һ��21���ַ������ַ�����v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTengxunRTDataSourceImp::IsInvalidTengxunRTData(const CWebData& WebDataReceived) {
	const string_view sv = WebDataReceived.GetStringViewData(21);

	if (sv.compare(_T("v_pv_none_match=\"1\";\n")) == 0) {
		ASSERT(WebDataReceived.GetBufferLength() == 21);
		return true;
	}
	else return false;
}