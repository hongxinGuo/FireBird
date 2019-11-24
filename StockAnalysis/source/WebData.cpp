#include"stdafx.h"

#include"globedef.h"
#include"Market.h"

#include"WebData.h"

CWebData::CWebData() noexcept {
  m_pCurrentPos = m_buffer;
  m_lCurrentPos = 0;
  m_lByteRead = 0;
  m_fReceivedData = false;
  m_strInquire = _T("");
  m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");
  m_fReceivedData = false;
  m_ReadingWebData = false;

#ifdef DEBUG
  m_fReportStatus = true;
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
    if (IsNeedProcessingCurrentWebData()) {
      ProcessCurrentWebData();
    }
    InquireNextWebData();
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// ���ǹ����̰߳档
// ���ù����̰߳棬���Լ���ס�̵߳�ѹ���͸��Ӷȣ�ͬʱ�����ݿ��Բ����������Ȳ�ѯ����ȡ������
//
///////////////////////////////////////////////////////////////////////////////////////
bool CWebData::GetDataByUsingThread(void) {
  if (!IsReadingWebData()) {
    InquireNextWebData();
    if (IsNeedProcessingCurrentWebData()) {
      while (!IsWebDataReceived()) Sleep(100);
      if (IsReadingSucceed()) {
        ResetCurrentPos();
        if (SucceedReadingAndStoringOneWebData()) {
          ProcessCurrentWebData();
        }
      }
    }
  }
  return true;
}

void CWebData::ProcessCurrentWebData(void) {
  if (IsWebDataReceived()) {
    if (IsReadingSucceed()) { //����ͨ��һ��˳����
      ResetCurrentPos();
      if (SucceedReadingAndStoringWebData()) {
        ProcessWebDataStored();
      }
    }
  }
  else {  // ����ͨ�ų��ִ���
    ReportCommunicationError();
  }
}

///////////////////////////////////////////////////////////////////////
//
// ��ΪĬ�ϵĶ�ȡ�洢���������б�Ҫ���̳���Ҳ���Ը��Ǵ˺���
//
///////////////////////////////////////////////////////////////////////
bool CWebData::SucceedReadingAndStoringWebData(void) {
  int iNumberOfDataReadAndStored = 0;
  ReadPrefix();
  while (!IsReadingFinished()) {
    if (!SucceedReadingAndStoringOneWebData()) {
      ReportDataError();
      return false;  // ��������ݳ����⣬�׵����á�
    }
    iNumberOfDataReadAndStored++;
  }
  if (m_fReportStatus) ReportStatus(iNumberOfDataReadAndStored);
  return true;
}

bool CWebData::ReadPrefix(void) {
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