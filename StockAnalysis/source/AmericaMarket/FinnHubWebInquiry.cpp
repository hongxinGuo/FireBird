#include"pch.h"
#include"globedef.h"
#include"Thread.h"

#include "FinnhubWebInquiry.h"
#include"WorldMarket.h"

using namespace std;
#include<thread>

//
//m_strWebDataInquireSuffix = _T("&token=bv4ac1n48v6tcp17l5cg"); // ��Կ�������
//m_strWebDataInquireSuffix = _T("&token=sandbox_bv8t3mn48v6rnm7c6aug"); // sandbox����Կ.sandboxֻ�����ڲ��ԣ�������Ϊα���ݡ�
// ������ǵڶ���,����dell240��������
//m_strWebDataInquireSuffix = _T("&token=bv985d748v6ujthqfke0"); // ��Կ�������
//m_strWebDataInquireSuffix = _T("&token=sandbox_bv985d748v6ujthqfkeg"); // sandbox����Կ.sandboxֻ�����ڲ��ԣ�������Ϊα���ݡ�

CFinnhubWebInquiry::CFinnhubWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T(""); // finnhub�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�

  // finnhub������һ���˻�ͬʱ�������������ϣ��ʶ�ʹ�������˻��������ܷ�ܿ��ٶ����ơ�
  // �п���һ����ַֻ����һ���˻����У������������˻��ķ���Ҳ�������á�
  if (m_strWebDataInquireSuffix.GetLength() < 5) {
#ifdef DEBUG
    m_strWebDataInquireSuffix = _T("&token=bv4ac1n48v6tcp17l5cg"); // ���԰�ʹ��ymail�˻�
    //m_strWebDataInquireSuffix = _T("&token=sandbox_bv8t3mn48v6rnm7c6aug"); // sandbox����Կ.sandboxֻ�����ڲ��ԣ�������Ϊα���ݡ�
#else
    //m_strWebDataInquireSuffix = _T("&token=bv4ac1n48v6tcp17l5cg"); // ���԰�ʹ��ymail�˻�
    m_strWebDataInquireSuffix = _T("&token=bv985d748v6ujthqfke0"); // ���а�ʹ��hotmail�˻�
  //m_strWebDataInquireSuffix = _T("&token=sandbox_bv985d748v6ujthqfkeg"); // sandbox����Կ.sandboxֻ�����ڲ��ԣ�������Ϊα���ݡ�
#endif // DEBUG
  }
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
  CString str = _T("");
  static int s_iCount = 5;

  switch (gl_pWorldMarket->GetCurrentFinnhubPrefixIndex()) {
  case __COMPANY_SYMBOLS__:
  // �����м䴮
  break;
  case __STOCK_QUOTE__:
  break;
  default:
  break;
  }

  return str;
}

void CFinnhubWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadFinnhubData, this);
  thread1.detach();
}

bool CFinnhubWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("����%d������ʵʱ����\n", lNumberOfData);
  return true;
}