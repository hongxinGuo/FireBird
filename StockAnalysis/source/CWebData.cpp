#include"stdafx.h"

#include"globedef.h"
#include"Market.h"

#include"CWebData.h"

bool CWebData::GetWebData(void)
{
  if (!IsReadingWebData()) {
    ProcessWebData();
    InquireNextWebData();
  }
  return true;
}

void CWebData::ProcessWebData(void) {
  CRTDataPtr pRTData = nullptr;
  char* pCurrentPos = nullptr;

  if (IsWebDataReceived()) {
    if (IsReadingSucceed()) { //����ͨ��һ��˳����
      pCurrentPos = m_buffer;
      long  iCount = 0;
      while (iCount < m_lByteRead) { // ����ʵʱ���ݻ���û�д��󣬲���Ҫ�׵����һ�������ˡ�
        if (!ReadAndSaveWebData(pCurrentPos, iCount)) {
          ReportDataError();
          iCount = m_lByteRead; // ��������ݿ��ܳ����⣬�׵����á�
        }
      }
      // ������յ���ʵʱ����
      gl_ChinaStockMarket.ProcessRTDataReceivedFromWeb();
    }
    else {  // ����ͨ�ų��ִ���
      ReportCommunicationError();
    }
  }
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