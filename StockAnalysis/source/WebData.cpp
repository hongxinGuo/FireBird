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
// 这是此类唯一的接口函数
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
// 这是工作线程版。
// 采用工作线程版，可以减少住线程的压力和复杂度，同时对数据可以采用正常的先查询后提取方法。
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
    if (IsReadingSucceed()) { //网络通信一切顺利？
      ResetCurrentPos();
      if (SucceedReadingAndStoringWebData()) {
        ProcessWebDataStored();
      }
    }
  }
  else {  // 网络通信出现错误
    ReportCommunicationError();
  }
}

///////////////////////////////////////////////////////////////////////
//
// 此为默认的读取存储函数，如有必要，继承类也可以覆盖此函数
//
///////////////////////////////////////////////////////////////////////
bool CWebData::SucceedReadingAndStoringWebData(void) {
  int iNumberOfDataReadAndStored = 0;
  ReadPrefix();
  while (!IsReadingFinished()) {
    if (!SucceedReadingAndStoringOneWebData()) {
      ReportDataError();
      return false;  // 后面的数据出问题，抛掉不用。
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
  TRACE("读入%d个实时数据\n", lNumberOfData);
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