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
// ���Ǵ���Ψһ�Ľӿں���
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
  ReadPrefix(pCurrentPos, iCount);
  while (!IsReadingFinished(pCurrentPos, iCount)) {
    if (!SucceedReadingAndStoringOneWebData(pCurrentPos, iCount)) {
      ReportDataError();
      return false;  // ��������ݳ����⣬�׵����á�
    }
    i++;
  }
  TRACE("����%d��ʵʱ����\n", i);
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