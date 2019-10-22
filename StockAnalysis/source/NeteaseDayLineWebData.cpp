#include"stdafx.h"
#include"globedef.h"
#include"thread.h"
#include"market.h"

#include "NeteaseDayLineWebData.h"

bool CNeteaseDayLineWebData::sm_fCreatedOnce = false; // 初始时没有生成过实例
bool CNeteaseDayLineWebData::sm_fNeedProcessingCurrentWebData = true; // 初始时允许处理当前网络数据

CNeteaseDayLineWebData::CNeteaseDayLineWebData() : CWebDataBase() {
  if (sm_fCreatedOnce) ASSERT(0); // 如果已经生成过一个实例了，则报错
  else sm_fCreatedOnce = true;

  m_strWebDataInquirePrefix = _T("http://quotes.money.163.com/service/chddata.html?code=");
  m_strWebDataInquireSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
}

CNeteaseDayLineWebData::~CNeteaseDayLineWebData() {
}

bool CNeteaseDayLineWebData::IsNeedProcessingCurrentWebData(void)
{
  if (sm_fNeedProcessingCurrentWebData) return true;
  else return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网易的日线历史数据不需要前期处理，直接使用ProcessWebDataStored函数即可。故而此处只是将iCount增至m_lByteRead即可。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDayLineWebData::SucceedReadingAndStoringOneWebData(char*&, long& iCount)
{
  iCount = m_lByteRead; //
  return true;
}

void CNeteaseDayLineWebData::ProcessWebDataStored(void) {
  TRACE("股票%s日线数据为%d字节\n", gl_ChinaStockMarket.GetDownLoadingStockCodeStr(), m_lByteRead);
  ASSERT(m_lByteRead < 2048 * 1024);
  gl_ChinaStockMarket.ProcessDayLineData(m_buffer, m_lByteRead);
}

void CNeteaseDayLineWebData::ReportDataError(void)
{
  TRACE("网易日线历史数据有误,抛掉不用\n");
  CString str;
  str = _T("网易日线历史数据有误");
  gl_systemMessage.PushInformationMessage(str);
}

void CNeteaseDayLineWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ：hq.sinajs.cn\n");
  CString str;
  str = _T("Error reading http file ：http://quotes.money.163.com");
  gl_systemMessage.PushInformationMessage(str);
}

void CNeteaseDayLineWebData::InquireNextWebData(void)
{
  CString strMiddle = _T("");
  char buffer2[200];
  CString strStartDay;

  // 准备网易日线数据申请格式
  sm_fNeedProcessingCurrentWebData = gl_ChinaStockMarket.CreateNeteaseDayLineInquiringStr(strMiddle, strStartDay);
  if (sm_fNeedProcessingCurrentWebData) {
    strMiddle += _T("&start=");
    strMiddle += strStartDay;
    strMiddle += _T("&end=");
    sprintf_s(buffer2, "%8d", gl_systemTime.GetDay());
    strMiddle += buffer2;
    CreateTotalInquiringString(strMiddle);
  }
  SetWebDataReceived(false);
  SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
  StartReadingThread();
}

int CNeteaseDayLineWebData::GetInquiringStockStr(CString& strInquire) {
  return gl_ChinaStockMarket.GetSinaInquiringStockStr(strInquire);
}

void CNeteaseDayLineWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadNeteaseDayLine, nullptr);
}