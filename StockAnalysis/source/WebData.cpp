#include"stdafx.h"

#include"globedef.h"
#include"Market.h"

#include"WebData.h"

CWebData::CWebData() noexcept {
  m_pCurrentPos = m_buffer;
  m_lCurrentPos = 0;
  m_lByteRead = 0;
  m_fSucceed = true;
  m_strInquire = _T("");
  m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");

  m_fReportStatus = false;
}

/////////////////////////////////////////////////////////////////////////
//
// 这是此类唯一的接口函数
//
//////////////////////////////////////////////////////////////////////////
bool CWebData::GetWebData(void)
{
  if (!IsReadingWebData()) {
    if (IsNeedProcessingCurrentWebData()) ProcessCurrentWebData();
    InquireNextWebData();
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
bool CWebData::SucceedReadingAndStoringWebData(void) {
  char* pCurrentPos = m_buffer;
  long  iCount = 0;
  int i = 0;
  ReadPrefix();
  while (!IsReadingFinished()) {
    if (!SucceedReadingAndStoringOneWebData()) {
      ReportDataError();
      return false;  // 后面的数据出问题，抛掉不用。
    }
    i++;
  }
  if (m_fReportStatus) ReportStatus(i);
  return true;
}

bool CWebData::ReadPrefix(void)
{
  return true;
}

bool CWebData::IsReadingFinished(void)
{
  if (m_lCurrentPos < m_lByteRead) return false;
  else return true;
}

bool CWebData::ReportStatus(long lNumberOfData)
{
  TRACE("读入%d个实时数据\n", lNumberOfData);
  return true;
}

void CWebData::CreateTotalInquiringString(CString strMiddle)
{
  m_strInquire = m_strWebDataInquirePrefix + strMiddle + m_strWebDataInquireSuffix;
}

void CWebData::TESTSetBuffer(char* buffer, long lTotalNumber)
{
  long i;
  for (i = 0; i < lTotalNumber; i++) {
    m_buffer[i] = buffer[i];
  }
  m_buffer[lTotalNumber] = 0x000;
  m_lByteRead = lTotalNumber;
  m_pCurrentPos = m_buffer;
  m_lCurrentPos = 0;
}