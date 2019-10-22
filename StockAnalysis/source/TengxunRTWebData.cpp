#include"stdafx.h"
#include"globedef.h"
#include"Market.h"

#include"thread.h"
#include "TengxunRTWebData.h"

bool CTengxunRTWebData::sm_fCreatedOnce = false; // ��ʼʱû�����ɹ�ʵ��

CTengxunRTWebData::CTengxunRTWebData() : CWebData() {
  if (sm_fCreatedOnce) ASSERT(0); // ����Ѿ����ɹ�һ��ʵ���ˣ��򱨴�
  else sm_fCreatedOnce = true;

  m_strWebDataInquirePrefix = _T("http://qt.gtimg.cn/q=");
  m_strWebDataInquireSuffix = _T("");
}

CTengxunRTWebData::~CTengxunRTWebData() {
}

bool CTengxunRTWebData::SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount)
{
  CRTDataPtr pRTData = make_shared<CRTData>();
  if (pRTData->ReadTengxunData(pCurrentPos, iCount)) {
    //gl_QueueRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
    return true;
  }
  return false;
}

void CTengxunRTWebData::ProcessWebDataStored(void) {
  //gl_ChinaStockMarket.ProcessRTDataReceivedFromWeb();
}

void CTengxunRTWebData::ReportDataError(void)
{
  TRACE("��Ѷʵʱ��������,�׵�����\n");
  CString str;
  str = _T("��Ѷʵʱ��������");
  gl_systemMessage.PushInformationMessage(str);
}

void CTengxunRTWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ��http://qt.gtimg.cn\n");
  CString str;
  str = _T("Error reading http file ��http://qt.gtimg.cn");
  gl_systemMessage.PushInformationMessage(str);
}

void CTengxunRTWebData::InquireNextWebData(void)
{
  CString strMiddle = _T("");
  ASSERT(gl_ChinaStockMarket.SystemReady());

  // ������һ���ι�Ʊʵʱ����
  GetInquiringStockStr(strMiddle);
  CreateTotalInquiringString(strMiddle);
  SetWebDataReceived(false);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

int CTengxunRTWebData::GetInquiringStockStr(CString& strInquire) {
  return gl_ChinaStockMarket.GetTengxunInquiringStockStr(strInquire);
}

void CTengxunRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadTengxunRTData, nullptr);
}