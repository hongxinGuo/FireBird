#include"stdafx.h"

#include"globedef.h"
#include"Market.h"

#include"WebDataBase.h"

CWebDataBase::CWebDataBase() noexcept {
  m_lByteRead = 0;
  m_fSucceed = true;
  m_strInquire = _T("");
  m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");

  m_fReadingWebData = false;
  m_fWebDataReceived = false;
}

/////////////////////////////////////////////////////////////////////////
//
// 这是此类唯一的接口函数
//
//////////////////////////////////////////////////////////////////////////
bool CWebDataBase::GetWebData(void)
{
  if (!IsReadingWebData()) {
    if (IsNeedProcessingCurrentWebData()) ProcessCurrentWebData();
    InquireNextWebData();
  }
  return true;
}

void CWebDataBase::ProcessCurrentWebData(void) {
  if (IsWebDataReceived()) {
    if (IsReadingSucceed()) { //网络通信一切顺利？
      if (SucceedReadingAndStoringWebData()) {
        ProcessWebDataStored();
      }
    }
    else {  // 网络通信出现错误
      ReportCommunicationError();
    }
  }
}

///////////////////////////////////////////////////////////////////////
//
// 此为默认的读取存储函数，如有必要，继承类也可以覆盖此函数
//
///////////////////////////////////////////////////////////////////////
bool CWebDataBase::SucceedReadingAndStoringWebData(void) {
  char* pCurrentPos = m_buffer;
  long  iCount = 0;
  int i = 0;
  while (iCount < m_lByteRead) {
    if (!SucceedReadingAndStoringOneWebData(pCurrentPos, iCount)) {
      ReportDataError();
      return false;  // 后面的数据出问题，抛掉不用。
    }
    i++;
  }
  TRACE("读入%d个实时数据\n", i);
  return true;
}

void CWebDataBase::SetWebDataReceived(bool fFlag) {
  CSingleLock singleLock(&m_WebDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fWebDataReceived = fFlag;
    singleLock.Unlock();
  }
}

bool CWebDataBase::IsWebDataReceived(void) {
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

void CWebDataBase::SetReadingWebData(bool fFlag) {
  CSingleLock singleLock(&m_ReadingWebDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fReadingWebData = fFlag;
    singleLock.Unlock();
  }
}

bool CWebDataBase::IsReadingWebData(void) {
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

void CWebDataBase::CreateTotalInquiringString(CString strMiddle)
{
  m_strInquire = m_strWebDataInquirePrefix + strMiddle + m_strWebDataInquireSuffix;
}