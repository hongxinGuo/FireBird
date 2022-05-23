#include"pch.h"
#include"globedef.h"
#include"Thread.h"

#include "FinnhubWebInquiry.h"
#include"WorldMarket.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

CFinnhubWebInquiry::CFinnhubWebInquiry() : CVirtualWebInquiry() {
	m_strWebDataInquirePrefix = _T(""); // finnhub�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�
	m_strWebDataInquireSuffix = _T("");
	m_strConnectionName = _T("Finnhub");
	m_lInquiringNumber = 1; // Finnhubʵʱ���ݲ�ѯ����Ĭ��ֵ
}

CFinnhubWebInquiry::~CFinnhubWebInquiry() {
}

bool CFinnhubWebInquiry::PrepareNextInquiringStr(void) {
	CWebRTDataPtr pRTData = nullptr;

	CString strMiddle = _T("");

	// ������һ���ι�Ʊʵʱ���ݡ� ������������ȡ��ʹ��FinnhubMarket
	// ����Finnhub�ṩ�������ݣ���ÿ�����ݷֱ������ȡ���ᵼ�³���̫�����ȡ�����ʶ��ڴ˷��ࡣ

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
CString CFinnhubWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
	// do nothing
	return _T("");
}

bool CFinnhubWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d��Finnhub����\n", lNumberOfData);
	return true;
}

void CFinnhubWebInquiry::PrepareReadingWebData(void) {
	ASSERT(!gl_pWorldMarket->IsFinnhubDataReceived());
}

void CFinnhubWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	while (gl_WebInquirer.FinnhubDataSize() > 0) gl_WebInquirer.PopFinnhubData();
	gl_pWorldMarket->SetFinnhubInquiring(false); // �������̳߳��ֹ���ʱ����Ҫ���Quandl���������־��
}

void CFinnhubWebInquiry::UpdateAfterReadingWebData(void) {
	gl_pWorldMarket->SetFinnhubDataReceived(true); // �������������ݺ����״̬��
}

void CFinnhubWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_WebInquirer.PushFinnhubData(pWebDataBeStored);
}