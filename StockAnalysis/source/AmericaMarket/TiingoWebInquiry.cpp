#include"pch.h"
#include"globedef.h"
#include"Thread.h"

#include "TiingoWebInquiry.h"
#include"WorldMarket.h"

using namespace std;
#include<thread>

//https://api.tiingo.com/api/
//m_strWebDataInquireSuffix = _T("&token=859bd66ca24b2a81a2b5f4de6616e2c408b2a769"); // ��Կ�������
//m_strWebDataInquireSuffix = _T("&token=c897a00b7cfc2adffc630d23befd5316a4683156"); // ��Կ�������
CTiingoWebInquiry::CTiingoWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T(""); // finnhub�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�

  if (m_strWebDataInquireSuffix.GetLength() < 5) {
#ifdef DEBUG
    m_strWebDataInquireSuffix = _T("&token=859bd66ca24b2a81a2b5f4de6616e2c408b2a769"); // ���԰�ʹ��ymail�˻�
#else
    m_strWebDataInquireSuffix = _T("&token=c897a00b7cfc2adffc630d23befd5316a4683156"); // ���а�ʹ��hotmail�˻�
#endif // DEBUG
  }
  m_strConnectionName = _T("Tiingo");
  m_lInquiringNumber = 1; // Finnhubʵʱ���ݲ�ѯ����Ĭ��ֵ
}

CTiingoWebInquiry::~CTiingoWebInquiry() {
}

bool CTiingoWebInquiry::PrepareNextInquiringStr(void) {
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
CString CTiingoWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
  // do nothing

  return _T("");
}

void CTiingoWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadTiingoData, this);
  thread1.detach();
}

bool CTiingoWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("����%d��Tiingo����\n", lNumberOfData);
  return true;
}