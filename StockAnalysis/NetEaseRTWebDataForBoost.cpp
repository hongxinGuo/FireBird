#include"stdafx.h"
#include"globedef.h"
#include"thread.h"
#include"market.h"

#include "NetEaseRTWebDataForBoost.h"

CNetEaseRTWebDataForBoost::CNetEaseRTWebDataForBoost() : CWebData() {
  m_fNeedProcessingCurrentWebData = true;

  m_strWebDataInquirePrefix = _T("http://api.money.126.net/data/feed/");
  m_strWebDataInquireSuffix = _T("");
}

CNetEaseRTWebDataForBoost::~CNetEaseRTWebDataForBoost() {
}

bool CNetEaseRTWebDataForBoost::IsNeedProcessingCurrentWebData(void)
{
  if (m_fNeedProcessingCurrentWebData) return true;
  else return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网易的日线历史数据不需要前期处理，直接使用ProcessWebDataStored函数即可。故而此处只是将iCount增至m_lByteRead即可。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNetEaseRTWebDataForBoost::SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount)
{
  pCurrentPos[m_lByteRead] = 0x000;
  iCount = m_lByteRead; //
  pCurrentPos += m_lByteRead;
  return true;
}

void CNetEaseRTWebDataForBoost::ProcessWebDataStored(void) {
  TRACE(_T("股票%s日线数据为%d字节\n"), m_strDownLoadingStockCode, m_lByteRead);
  ASSERT(m_lByteRead < 2048 * 1024);
  gl_ChinaStockMarket.ReadNeteaseRTDataUsingPropertyTree(this);
}

void CNetEaseRTWebDataForBoost::ReportDataError(void)
{
  TRACE("网易实时数据有误,抛掉不用\n");
  CString str;
  str = _T("网易实时数据有误");
  gl_systemMessage.PushInformationMessage(str);
}

void CNetEaseRTWebDataForBoost::ReportCommunicationError(void)
{
  TRACE("Error reading http file ：http://api.money.126.net/data/feed/\n");
  CString str;
  str = _T("Error reading http file ：http://api.money.126.net/data/feed/");
  gl_systemMessage.PushInformationMessage(str);
}

void CNetEaseRTWebDataForBoost::InquireNextWebData(void)
{
  CString strMiddle = _T("");

  // 申请下一批次股票实时数据
  if (!gl_ChinaStockMarket.SystemReady() || gl_ChinaStockMarket.IsUsingNeteaseRTDataReceiverAsTester()) { // 如果系统尚未准备好，则使用全局股票池
    gl_ChinaStockMarket.CreateNeteaseRTDataInquiringStr(strMiddle);
  }
  else { // 开市时使用今日活跃股票池
    GetInquiringStr(strMiddle);
  }
  CreateTotalInquiringString(strMiddle);

  SetWebDataReceived(false);
  SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
  StartReadingThread();
}

int CNetEaseRTWebDataForBoost::GetInquiringStr(CString& strInquire) {
  return gl_ChinaStockMarket.GetNeteaseInquiringStockStr(strInquire);
}

void CNetEaseRTWebDataForBoost::StartReadingThread(void) {
  AfxBeginThread(ThreadReadNeteaseRTData, nullptr);
}