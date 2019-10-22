#include"stdafx.h"
#include"globedef.h"
#include"thread.h"
#include"market.h"

#include "NeteaseDayLineWebData.h"

bool CNeteaseDayLineWebData::sm_fCreatedOnce = false; // ��ʼʱû�����ɹ�ʵ��

CNeteaseDayLineWebData::CNeteaseDayLineWebData() : CWebData() {
  if (sm_fCreatedOnce) ASSERT(0); // ����Ѿ����ɹ�һ��ʵ���ˣ��򱨴�
  else sm_fCreatedOnce = true;

  m_strWebDataInquirePrefix = _T("http://quotes.money.163.com/service/chddata.html?code=");
  m_strWebDataInquireSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
}

CNeteaseDayLineWebData::~CNeteaseDayLineWebData() {
}

bool CNeteaseDayLineWebData::SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount)
{
  CRTDataPtr pRTData = make_shared<CRTData>();
  if (pRTData->ReadSinaData(pCurrentPos, iCount)) {
    gl_QueueRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
    return true;
  }
  return false;
}

void CNeteaseDayLineWebData::ProcessWebDataStored(void) {
  gl_ChinaStockMarket.ProcessRTDataReceivedFromWeb();
}

void CNeteaseDayLineWebData::ReportDataError(void)
{
  TRACE("��������,�׵�����\n");
  CString str;
  str = _T("��������");
  gl_systemMessage.PushInformationMessage(str);
}

void CNeteaseDayLineWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ��hq.sinajs.cn\n");
  CString str;
  str = _T("Error reading http file ��hq.sinajs.cn");
  gl_systemMessage.PushInformationMessage(str);
}

void CNeteaseDayLineWebData::InquireNextWebData(void)
{
  CString strTemp;
  char buffer2[200];
  CString strStartDay, strRead;
  static bool sfFoundStock = false;

  // ׼�������������������ʽ
  strRead = m_strWebDataInquirePrefix;
  sfFoundStock = gl_ChinaStockMarket.CreateNeteaseDayLineInquiringStr(strRead, strStartDay);
  if (sfFoundStock) {
    strRead += _T("&start=");
    strRead += strStartDay;
    strRead += _T("&end=");
    sprintf_s(buffer2, "%8d", gl_systemTime.GetDay());
    strRead += buffer2;
    strRead += m_strWebDataInquireSuffix;

    m_strInquire = strRead;
  }
  SetWebDataReceived(false);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

int CNeteaseDayLineWebData::GetInquiringStockStr(CString& strInquire) {
  return gl_ChinaStockMarket.GetSinaInquiringStockStr(strInquire);
}

void CNeteaseDayLineWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadNeteaseDayLine, nullptr);
}