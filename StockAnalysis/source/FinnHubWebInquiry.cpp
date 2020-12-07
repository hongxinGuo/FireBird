#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"

#include "FinnhubWebInquiry.h"
#include"AmericaStakeMarket.h"

using namespace std;
#include<thread>

CFinnhubWebInquiry::CFinnhubWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("https://finnhub.io/api/v1/quote?symbol=");
  m_strWebDataInquireSuffix = _T("&token=bv4ac1n48v6tcp17l5cg"); // ��Կ�������
  //m_strWebDataInquireSuffix = _T(""); // ��Կ�������
  m_strConnection = _T("FinnHubRT");
  m_lInquiringNumber = 1; // FinnHubʵʱ���ݲ�ѯ����Ĭ��ֵ
}

CFinnhubWebInquiry::~CFinnhubWebInquiry() {
}

bool CFinnhubWebInquiry::PrepareNextInquiringStr(void) {
  CWebRTDataPtr pRTData = nullptr;

  CString strMiddle = _T("");

  // ������һ���ι�Ʊʵʱ���ݡ� ������������ȡ��ʹ��FinnHubMarket
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
  CString str;
  static int s_iCount = 5;

  switch (gl_pAmericaStakeMarket->GetCurrentPrefixIndex()) {
  case __COMPANY_SYMBOLS__:
  // �����м䴮
  break;
  case __QUOTE__:
  break;
  default:
  break;
  }

  return str;
}

void CFinnhubWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadFinnHubData, this);
  thread1.detach();
}

bool CFinnhubWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("����%d������ʵʱ����\n", lNumberOfData);
  return true;
}