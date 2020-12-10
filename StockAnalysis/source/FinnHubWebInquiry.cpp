#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"

#include "FinnhubWebInquiry.h"
#include"AmericaStakeMarket.h"

using namespace std;
#include<thread>

CFinnhubWebInquiry::CFinnhubWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T(""); // finnhub�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�
  m_strWebDataInquireSuffix = _T("&token=bv4ac1n48v6tcp17l5cg"); // ��Կ�������
  //m_strWebDataInquireSuffix = _T("&token=sandbox_bv8t3mn48v6rnm7c6aug"); // sandbox����Կ.sandboxֻ�����ڲ��ԣ�������Ϊα���ݡ�
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
  case __STAKE_QUOTE__:
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