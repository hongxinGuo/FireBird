#include"pch.h"

#include"ChinaMarket.h"

#include "TengxunRTWebInquiry.h"
#include"TengxunRTDataSource.h"

using namespace std;
#include<thread>

CTengxunRTWebInquiry::CTengxunRTWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T("http://qt.gtimg.cn/q=");
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("TengxunRT");
	m_lInquiringNumber = 900; // ��Ѷʵʱ���ݲ�ѯĬ��ֵ

	ConfigerateSession();
}

CTengxunRTWebInquiry::~CTengxunRTWebInquiry() {
}

bool CTengxunRTWebInquiry::PrepareNextInquiringString(void) {
	CString strMiddle = _T("");
	ASSERT(gl_pChinaMarket->IsSystemReady());
	// ������һ���ι�Ʊʵʱ���ݡ�
	// ������Ѷʵʱ����ʱ��������������ڵĹ�Ʊ���룬�������᷵��v_pv_none_match="1";������ϵͳ���ϣ�
	// �ʶ�����ֻʹ����Ч��Ʊ���롣
	strMiddle = GetNextInquiringMiddleString(m_lInquiringNumber); // ʹ�û�Ծ��Ʊ��
	CreateTotalInquiringString(strMiddle);
	return true;
}

CString CTengxunRTWebInquiry::GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) {
	ASSERT(gl_pChinaMarket->IsSystemReady());
	return gl_pChinaMarket->GetNextTengxunStockInquiringMiddleStr(lTotalNumber);
}

void CTengxunRTWebInquiry::ConfigerateSession(void)
{
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000); // ���������Tengxunʵʱ���ݽ���ʱ�����Ϊ300���롣
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 1000); // ���ý��ճ�ʱʱ��Ϊ1000����
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 300); // ���÷��ͳ�ʱʱ��Ϊ100����
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1������
}

bool CTengxunRTWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d����Ѷʵʱ����\n", lNumberOfData);
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����б���ѯ�Ĺ�Ʊ��Ϊ�����й�Ʊʱ����Ѷʵʱ��Ʊ�������᷵��һ��21���ַ������ַ�����v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTengxunRTWebInquiry::IsTengxunRTDataInvalid(CWebData& WebDataReceived) {
	char buffer[50]{};
	char* pBuffer = buffer;
	CString strInvalidStock = _T("v_pv_none_match=\"1\";\n"); // ��Ϊ��Ч��Ʊ��ѯ�������ݸ�ʽ����21���ַ�

	WebDataReceived.GetData(pBuffer, 21, WebDataReceived.GetCurrentPos());
	buffer[21] = 0x000;
	CString str1 = buffer;

	if (str1.Compare(strInvalidStock) == 0) {
		ASSERT(WebDataReceived.GetBufferLength() == 21);
		return true;
	}
	else return false;
}

void CTengxunRTWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_pTengxunRTDataSource->StoreReceivedData(pWebDataBeStored);
}

void CTengxunRTWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	while (gl_pTengxunRTDataSource->GetReceivedDataSize() > 0) gl_pTengxunRTDataSource->GetReceivedData();
	gl_pTengxunRTDataSource->SetInquiring(false); // �������̳߳��ֹ���ʱ����Ҫ���Quandl���������־��
}

void CTengxunRTWebInquiry::UpdateStatusAfterReadingWebData(void) {
	gl_pTengxunRTDataSource->SetDataReceived(true);
}