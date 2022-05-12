#include"pch.h"
#include"globedef.h"
#include"Thread.h"

#include "TiingoWebInquiry.h"
#include"WorldMarket.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

CTiingoWebInquiry::CTiingoWebInquiry() : CVirtualWebInquiry() {
	m_strHeaders = _T("timeout:(5,200)\r\n");
	m_strWebDataInquirePrefix = _T(""); // Tiingo�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�
	m_strWebDataInquireSuffix = _T("");
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

void CTiingoWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_WebInquirer.PushTiingoData(pWebDataBeStored);
}