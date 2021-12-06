#include"pch.h"
#include"globedef.h"
#include"Thread.h"

#include "QuandlWebInquiry.h"
#include"WorldMarket.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

//  api_key=zBMXMyoTyiy_N3pMb3ex
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ��Կ�������
// ������ǵڶ���,����dell240��������
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ��Կ�������

CQuandlWebInquiry::CQuandlWebInquiry() : CVirtualWebInquiry() {
	m_strWebDataInquirePrefix = _T(""); // Quandl�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�

	// finnhub������һ���˻�ͬʱ�������������ϣ��ʶ�ʹ�������˻��������ܷ�ܿ��ٶ����ơ�
	// �п���һ����ַֻ����һ���˻����У������������˻��ķ���Ҳ�������á�
	if (m_strWebDataInquireSuffix.GetLength() < 5) {
#ifdef DEBUG
		m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ���԰�ʹ��ymail�˻�
#else
		m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ���а�ʹ��hotmail�˻�
#endif // DEBUG
	}
	m_strConnectionName = _T("Quandl");
	m_lInquiringNumber = 1; // Quandlʵʱ���ݲ�ѯ����Ĭ��ֵ
}

CQuandlWebInquiry::~CQuandlWebInquiry() {
}

bool CQuandlWebInquiry::PrepareNextInquiringStr(void) {
	CWebRTDataPtr pRTData = nullptr;

	CString strMiddle = _T("");

	// ������һ���ι�Ʊʵʱ���ݡ� ������������ȡ��ʹ��QuandlMarket
	// ����Quandl�ṩ�������ݣ���ÿ�����ݷֱ������ȡ���ᵼ�³���̫�����ȡ�����ʶ��ڴ˷��ࡣ

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
CString CQuandlWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
	CString str = _T("");
	static int s_iCount = 5;

	switch (gl_pWorldMarket->GetCurrentQuandlInquiry().m_lInquiryIndex) {
	case __STOCK_SYMBOLS__:
		// �����м䴮
		break;
	case __STOCK_PRICE_QUOTE__:
		break;
	default:
		break;
	}

	return str;
}

bool CQuandlWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d������ʵʱ����\n", lNumberOfData);
	return true;
}

void CQuandlWebInquiry::PrepareReadingWebData(void) {
	ASSERT(!gl_pWorldMarket->IsQuandlDataReceived());
}

void CQuandlWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	while (gl_WebInquirer.GetQuandlDataSize() > 0) gl_WebInquirer.PopQuandlData();
	gl_pWorldMarket->SetQuandlInquiring(false); // �������̳߳��ֹ���ʱ����Ҫ���Quandl���������־��
	gl_systemMessage.PushErrorMessage(_T("Quandl�����̳߳���"));
}

void CQuandlWebInquiry::UpdateAfterReadingWebData(void) {
	gl_pWorldMarket->SetQuandlDataReceived(true); // �������������ݺ����״̬��
}

void CQuandlWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_WebInquirer.PushQuandlData(pWebDataBeStored);
}