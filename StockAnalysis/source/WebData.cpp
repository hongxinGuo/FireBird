#include"stdafx.h"

#include"globedef.h"
#include"Market.h"

#include"WebData.h"

CWebData::CWebData() noexcept {
  m_lByteRead = 0;
  m_fSucceed = true;
  m_strInquire = _T("");
  m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");
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
  ReadPrefix(pCurrentPos, iCount);
  while (!IsReadingFinished(pCurrentPos, iCount)) {
    if (!SucceedReadingAndStoringOneWebData(pCurrentPos, iCount)) {
      ReportDataError();
      return false;  // 后面的数据出问题，抛掉不用。
    }
    i++;
  }
  TRACE("读入%d个实时数据\n", i);
  return true;
}

bool CWebData::ReadPrefix(char*& pCurrentPos, long& iCount)
{
  return true;
}

bool CWebData::IsReadingFinished(const char* const pCurrentPos, const long iCount)
{
  if (iCount < m_lByteRead) return false;
  else return true;
}

void CWebData::CreateTotalInquiringString(CString strMiddle)
{
  m_strInquire = m_strWebDataInquirePrefix + strMiddle + m_strWebDataInquireSuffix;
}