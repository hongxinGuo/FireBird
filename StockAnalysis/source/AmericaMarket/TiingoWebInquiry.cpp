#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"

#include "TiingoWebInquiry.h"
#include"AmericaMarket.h"

using namespace std;
#include<thread>

//https://api.tiingo.com/api/
//m_strWebDataInquireSuffix = _T("&token=859bd66ca24b2a81a2b5f4de6616e2c408b2a769"); // ��Կ�������
CTiingoWebInquiry::CTiingoWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T(""); // finnhub�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�

#ifdef DEBUG
  m_strWebDataInquireSuffix = _T("&token=859bd66ca24b2a81a2b5f4de6616e2c408b2a769"); // ���԰�ʹ��ymail�˻�
#else
  m_strWebDataInquireSuffix = _T("&token=859bd66ca24b2a81a2b5f4de6616e2c408b2a769"); // ���а�ʹ��hotmail�˻�
#endif // DEBUG

  m_strConnection = _T("Tiingo");
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