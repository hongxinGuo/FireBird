#include"pch.h"
#include"globedef.h"
#include"Thread.h"

#include "TiingoWebInquiry.h"
#include"WorldMarket.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

//https://api.tiingo.com/api/
//m_strWebDataInquireSuffix = _T("&token=859bd66ca24b2a81a2b5f4de6616e2c408b2a769"); // ��Կ�������
//m_strWebDataInquireSuffix = _T("&token=c897a00b7cfc2adffc630d23befd5316a4683156"); // ��Կ�������
//m_strWebDataInquireSuffix = _T("&token=fad87279362b9e580e4fb364a263cda3c67336c8"); // hxguo111@hotmail.com
CTiingoWebInquiry::CTiingoWebInquiry() : CVirtualWebInquiry() {
	m_strWebDataInquirePrefix = _T(""); // Tiingo�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�

	if (m_strWebDataInquireSuffix.GetLength() < 5) {
#ifdef DEBUG
		m_strWebDataInquireSuffix = _T("&token=fad87279362b9e580e4fb364a263cda3c67336c8"); // ���԰�ʹ��hxguo111@hotmail.com�˻�
#else
		m_strWebDataInquireSuffix = _T("&token=fad87279362b9e580e4fb364a263cda3c67336c8"); // ���а�ʹ��hxguo111@hotmail.com�˻�
#endif // DEBUG
	}
	m_strConnectionName = _T("Tiingo");
	m_lInquiringNumber = 1; // Tiingoʵʱ���ݲ�ѯ����Ĭ��ֵ
}

CTiingoWebInquiry::~CTiingoWebInquiry() {
}

bool CTiingoWebInquiry::PrepareNextInquiringStr(void) {
	CWebRTDataPtr pRTData = nullptr;

	CString strMiddle = _T("");

	// ������һ���ι�Ʊʵʱ���ݡ� ������������ȡ��ʹ��TiingoMarket
	// ����Tiingo�ṩ�������ݣ���ÿ�����ݷֱ������ȡ���ᵼ�³���̫�����ȡ�����ʶ��ڴ˷��ࡣ

	// 1 ׼��ǰ׺�ַ���
	// 2. ׼���м��ַ���
	// 3. ׼����׺�ַ���
	//
	CreateTotalInquiringString(m_strWebDataInquireMiddle);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CTiingoWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
	// do nothing

	return _T("");
}

bool CTiingoWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d��Tiingo����\n", lNumberOfData);
	return true;
}

void CTiingoWebInquiry::PrepareReadingWebData(void) {
	ASSERT(!gl_pWorldMarket->IsTiingoDataReceived());
}

void CTiingoWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	while (gl_WebInquirer.GetTiingoDataSize() > 0) gl_WebInquirer.PopTiingoData();
	gl_pWorldMarket->SetTiingoInquiring(false); // �������̳߳��ֹ���ʱ����Ҫ���Tiingo���������־��
	gl_systemMessage.PushErrorMessage(_T("Tiingo�����̳߳���"));
}

void CTiingoWebInquiry::UpdateAfterReadingWebData(void) {
	gl_pWorldMarket->SetTiingoDataReceived(true); // �������������ݺ����״̬��
}

void CTiingoWebInquiry::StoreWebData(CWebDataPtr pWebDataReceived) {
	gl_WebInquirer.PushTiingoData(pWebDataReceived);
}