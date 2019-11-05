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

bool CNeteaseRTWebData::ReadPrefix(char*& pCurrentPos, long& iCount)
{
  static char buffer1[200];
  static CString strHeader = _T("_ntes_quote_callback({");

  strncpy_s(buffer1, pCurrentPos, 22); // 读入"_ntes_quote_callback"
  buffer1[22] = 0x000;
  CString str1;
  str1 = buffer1;
  if (strHeader.Compare(str1) != 0) { // 数据格式出错
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
    //gl_QueueRTData.PushRTData(pRTData); // 将此实时数据指针存入实时数据队列
    if (*pCurrentPos == ' ') { // 到结尾处了
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