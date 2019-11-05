#include"stdafx.h"
#include"globedef.h"
#include"Market.h"

#include"thread.h"
#include "NeteaseRTWebData.h"

bool CNeteaseRTWebData::sm_fCreatedOnce = false; // 初始时没有生成过实例

CNeteaseRTWebData::CNeteaseRTWebData() : CWebData() {
  if (sm_fCreatedOnce) ASSERT(0); // 如果已经生成过一个实例了，则报错
  else sm_fCreatedOnce = true;

  m_strWebDataInquirePrefix = _T("http://api.money.126.net/data/feed/");
  m_strWebDataInquireSuffix = _T("");
}

CNeteaseRTWebData::~CNeteaseRTWebData() {
}

bool CNeteaseRTWebData::SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount)
{
  CRTDataPtr pRTData = make_shared<CRTData>();
  if (pRTData->ReadNeteaseData(pCurrentPos, iCount)) {
    pRTData->SetDataSource(__NETEASE_RT_WEB_DATA__);
    //gl_QueueRTData.PushRTData(pRTData); // 将此实时数据指针存入实时数据队列
    return true;
  }
  return false;
}

void CNeteaseRTWebData::ProcessWebDataStored(void) {
  //将下面的函数移入定时调度处，每3秒执行一次即可。本函数无需做任何事情，
  //gl_ChinaStockMarket.DistributeRTDataReceivedFromWebToProperStock();
}

void CNeteaseRTWebData::ReportDataError(void)
{
  TRACE("网易实时数据有误,抛掉不用\n");
  CString str;
  str = _T("网易实时数据有误");
  gl_systemMessage.PushInformationMessage(str);
}

void CNeteaseRTWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ：http://api.money.126.net/data/feed/\n");
  CString str;
  str = _T("Error reading http file ：http://api.money.126.net/data/feed/");
  gl_systemMessage.PushInformationMessage(str);
}

void CNeteaseRTWebData::InquireNextWebData(void)
{
  CString strMiddle = _T("");
  ASSERT(gl_ChinaStockMarket.SystemReady());

  // 申请下一批次股票实时数据
  GetInquiringStr(strMiddle);
  CreateTotalInquiringString(strMiddle);
  SetWebDataReceived(false);
  SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
  StartReadingThread();
}

int CNeteaseRTWebData::GetInquiringStr(CString& strInquire) {
  return gl_ChinaStockMarket.GetNeteaseInquiringStockStr(strInquire);
}

void CNeteaseRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadNeteaseRTData, nullptr);
}