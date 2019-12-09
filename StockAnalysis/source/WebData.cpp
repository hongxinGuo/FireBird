#include"stdafx.h"
#include"Thread.h"

#include"globedef.h"
#include"Market.h"

#include"WebData.h"

CWebData::CWebData() {
  m_pCurrentPos = m_buffer;
  m_lCurrentPos = 0;
  m_lByteRead = 0;
  m_fReceivedData = false;
  m_strInquire = _T("");
  m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");
  m_fReadingWebData = false; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
  m_fWebDataReceived = false;// ʵʱ�����ѽ�����ϱ�ʶ

#ifdef DEBUG
  m_fReportStatus = false;
#else
  m_fReportStatus = false;
#endif
}

/////////////////////////////////////////////////////////////////////////
//
// ���Ǵ���Ψһ�Ľӿں���
//
//////////////////////////////////////////////////////////////////////////
bool CWebData::GetWebData(void) {
  if (!IsReadingWebData()) {
    InquireNextWebData();
  }
  return true;
}

bool CWebData::IsReadingFinished(void) {
  if (m_lCurrentPos < m_lByteRead) return false;
  else return true;
}

bool CWebData::ReportStatus(long lNumberOfData) {
  TRACE("����%d��ʵʱ����\n", lNumberOfData);
  return true;
}

void CWebData::CreateTotalInquiringString(CString strMiddle) {
  m_strInquire = m_strWebDataInquirePrefix + strMiddle + m_strWebDataInquireSuffix;
}

void CWebData::__TESTSetBuffer(char* buffer, long lTotalNumber) {
  long i;
  for (i = 0; i < lTotalNumber; i++) {
    m_buffer[i] = buffer[i];
  }
  m_buffer[lTotalNumber] = 0x000;
  m_lByteRead = lTotalNumber;
  m_pCurrentPos = m_buffer;
  m_lCurrentPos = 0;
}