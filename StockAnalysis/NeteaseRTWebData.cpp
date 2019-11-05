#include"stdafx.h"
#include"globedef.h"
#include"Market.h"

#include"thread.h"
#include "NeteaseRTWebData.h"

bool CNeteaseRTWebData::sm_fCreatedOnce = false; // ��ʼʱû�����ɹ�ʵ��

CNeteaseRTWebData::CNeteaseRTWebData() : CWebData() {
  if (sm_fCreatedOnce) ASSERT(0); // ����Ѿ����ɹ�һ��ʵ���ˣ��򱨴�
  else sm_fCreatedOnce = true;

  m_strWebDataInquirePrefix = _T("http://api.money.126.net/data/feed/");
  m_strWebDataInquireSuffix = _T("");
}

CNeteaseRTWebData::~CNeteaseRTWebData() {
}

bool CNeteaseRTWebData::ReadPrefix(char*& pCurrentPos, long& iCount)
{
  static char buffer1[200];
  static CString strHeader = _T("_ntes_quote_callback({");

  strncpy_s(buffer1, pCurrentPos, 22); // ����"_ntes_quote_callback"
  buffer1[22] = 0x000;
  CString str1;
  str1 = buffer1;
  if (strHeader.Compare(str1) != 0) { // ���ݸ�ʽ����
    return false;
  }
  pCurrentPos += 22;
  iCount += 22;

  return true;
}

bool CNeteaseRTWebData::SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount)
{
  CRTDataPtr pRTData = make_shared<CRTData>();
  if (pRTData->ReadNeteaseData(pCurrentPos, iCount)) {
    pRTData->SetDataSource(__NETEASE_RT_WEB_DATA__);
    //gl_QueueRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
    if (*pCurrentPos == ' ') { // ����β����
      pCurrentPos += 4;
      iCount += 4;
      ASSERT(iCount == m_lByteRead);
      return true;
    }
    else {
      ASSERT(*pCurrentPos == ',');
      pCurrentPos++;
      iCount++;
      return true;
    }
  }
  return false;
}

void CNeteaseRTWebData::ProcessWebDataStored(void) {
  //������ĺ������붨ʱ���ȴ���ÿ3��ִ��һ�μ��ɡ��������������κ����飬
  //gl_ChinaStockMarket.DistributeRTDataReceivedFromWebToProperStock();
}

void CNeteaseRTWebData::ReportDataError(void)
{
  TRACE("����ʵʱ��������,�׵�����\n");
  CString str;
  str = _T("����ʵʱ��������");
  gl_systemMessage.PushInformationMessage(str);
}

void CNeteaseRTWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ��http://api.money.126.net/data/feed/\n");
  CString str;
  str = _T("Error reading http file ��http://api.money.126.net/data/feed/");
  gl_systemMessage.PushInformationMessage(str);
}

void CNeteaseRTWebData::InquireNextWebData(void)
{
  CString strMiddle = _T("");
  ASSERT(gl_ChinaStockMarket.SystemReady());

  // ������һ���ι�Ʊʵʱ����
  GetInquiringStr(strMiddle);
  CreateTotalInquiringString(strMiddle);
  SetWebDataReceived(false);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

int CNeteaseRTWebData::GetInquiringStr(CString& strInquire) {
  return gl_ChinaStockMarket.GetNeteaseInquiringStockStr(strInquire);
}

void CNeteaseRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadNeteaseRTData, nullptr);
}