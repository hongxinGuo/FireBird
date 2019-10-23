#include"stdafx.h"
#include"globedef.h"
#include"thread.h"
#include"market.h"

#include "CrweberIndexWebData.h"

bool CCrweberIndexWebData::sm_fCreatedOnce = false; // ��ʼʱû�����ɹ�ʵ��

CCrweberIndexWebData::CCrweberIndexWebData() : CWebData() {
  if (sm_fCreatedOnce) ASSERT(0); // ����Ѿ����ɹ�һ��ʵ���ˣ��򱨴�
  else sm_fCreatedOnce = true;

  m_strWebDataInquirePrefix = _T("http://www.crweber.com");
  m_strWebDataInquireSuffix = _T("");
}

CCrweberIndexWebData::~CCrweberIndexWebData() {
}

bool CCrweberIndexWebData::SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount)
{
  iCount = m_lByteRead; //
  pCurrentPos += m_lByteRead;
  return true;
}

void CCrweberIndexWebData::ProcessWebDataStored(void) {
  //gl_ChinaStockMarket.ProcessRTDataReceivedFromWeb();
}

void CCrweberIndexWebData::ReportDataError(void)
{
  CString str;
  str = _T("crweber.com data error");
  gl_systemMessage.PushInformationMessage(str);
}

void CCrweberIndexWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ��www.crweber.com\n");
  CString str;
  str = _T("Error reading http file : http://www.crweber.com");
  gl_systemMessage.PushInformationMessage(str);
}

void CCrweberIndexWebData::InquireNextWebData(void)
{
  CString strMiddle = _T("");

  gl_SinaRTWebData.CreateTotalInquiringString(strMiddle);
  SetWebDataReceived(false);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

int CCrweberIndexWebData::GetInquiringStr(CString& strInquire) {
  strInquire = _T("");
  return 0;
}

void CCrweberIndexWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadCrweberIndex, nullptr);
}