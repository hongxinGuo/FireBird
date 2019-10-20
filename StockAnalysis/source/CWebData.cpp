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
    if (IsReadingSucceed()) { //网络通信一切顺利？
      pCurrentPos = m_buffer;
      long  iCount = 0;
      while (iCount < m_lByteRead) { // 新浪实时数据基本没有错误，不需要抛掉最后一组数据了。
        if (!ReadAndSaveWebData(pCurrentPos, iCount)) {
          ReportDataError();
          iCount = m_lByteRead; // 后面的数据可能出问题，抛掉不用。
        }
      }
      // 处理接收到的实时数据
      gl_ChinaStockMarket.ProcessRTDataReceivedFromWeb();
    }
    else {  // 网络通信出现错误
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
  return false; // 此分支不可能执行到，只为了消除编译器的警告而存在
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
  return false; // 此分支不可能执行到，只为了消除编译器的警告而存在
}