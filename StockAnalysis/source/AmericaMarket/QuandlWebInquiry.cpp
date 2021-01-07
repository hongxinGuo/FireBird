#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"

#include "QuandlWebInquiry.h"
#include"AmericaMarket.h"

using namespace std;
#include<thread>

//  api_key=zBMXMyoTyiy_N3pMb3ex
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ��Կ�������
// ������ǵڶ���,����dell240��������
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ��Կ�������

CQuandlWebInquiry::CQuandlWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T(""); // finnhub�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�

  // finnhub������һ���˻�ͬʱ�������������ϣ��ʶ�ʹ�������˻��������ܷ�ܿ��ٶ����ơ�
  // �п���һ����ַֻ����һ���˻����У������������˻��ķ���Ҳ�������á�
  if (m_strWebDataInquireSuffix.GetLength() < 5) {
#ifdef DEBUG
    m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ���԰�ʹ��ymail�˻�
#else
    m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ���а�ʹ��hotmail�˻�
#endif // DEBUG
  }
  m_strConnection = _T("Quandl");
  m_lInquiringNumber = 1; // Finnhubʵʱ���ݲ�ѯ����Ĭ��ֵ
}

CQuandlWebInquiry::~CQuandlWebInquiry() {
}

bool CQuandlWebInquiry::PrepareNextInquiringStr(void) {
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
CString CQuandlWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
  CString str;
  static int s_iCount = 5;

  switch (gl_pAmericaMarket->GetCurrentPrefixIndex()) {
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

void CQuandlWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadQuandlData, this);
  thread1.detach();
}

bool CQuandlWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("����%d������ʵʱ����\n", lNumberOfData);
  return true;
}