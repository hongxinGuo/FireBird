#include"stdafx.h"

#include"globedef.h"
#include"Market.h"

#include"WebData.h"

CWebData::CWebData() noexcept {
  m_lByteRead = 0;
  m_fSucceed = true;
  m_strInquire = _T("");
  m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");

  m_fReadingWebData = false;
  m_fWebDataReceived = false;
}

/////////////////////////////////////////////////////////////////////////
//
// ���Ǵ���Ψһ�Ľӿں���
//
//////////////////////////////////////////////////////////////////////////
bool CWebData::GetWebData(void)
{
  if (!IsReadingWebData()) {
    ProcessCurrentWebData();
    InquireNextWebData();
  }
  return true;
}

void CWebData::ProcessCurrentWebData(void) {
  if (IsWebDataReceived()) {
    if (IsReadingSucceed()) { //����ͨ��һ��˳����
      if (SucceedReadingAndStoringWebData()) {
        ProcessWebDataStored();
      }
    }
    else {  // ����ͨ�ų��ִ���
      ReportCommunicationError();
    }
  }
}

///////////////////////////////////////////////////////////////////////
//
// ��ΪĬ�ϵĶ�ȡ�洢���������б�Ҫ���̳���Ҳ���Ը��Ǵ˺���
//
///////////////////////////////////////////////////////////////////////
bool CWebData::SucceedReadingAndStoringWebData(void) {
  char* pCurrentPos = m_buffer;
  long  iCount = 0;
  int i = 0;
  while (iCount < m_lByteRead) {
    if (!SucceedReadingAndStoringOneWebData(pCurrentPos, iCount)) {
      ReportDataError();
      return false;  // ��������ݳ����⣬�׵����á�
    }
    i++;
  }
  TRACE("����%d������ʵʱ����\n", i);
  return true;
}

void CWebData::SetWebDataReceived(bool fFlag) {
  CSingleLock singleLock(&m_WebDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fWebDataReceived = fFlag;
    singleLock.Unlock();
  }
}

bool CWebData::IsWebDataReceived(void) {
  CSingleLock singleLock(&m_WebDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fWebDataReceived;
    singleLock.Unlock();
    return fFlag;
  }
  ASSERT(0);
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
}

void CWebData::SetReadingWebData(bool fFlag) {
  CSingleLock singleLock(&m_ReadingWebDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fReadingWebData = fFlag;
    singleLock.Unlock();
  }
}

bool CWebData::IsReadingWebData(void) {
  CSingleLock singleLock(&m_ReadingWebDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fReadingWebData;
    singleLock.Unlock();
    return fFlag;
  }
  ASSERT(0);
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
}