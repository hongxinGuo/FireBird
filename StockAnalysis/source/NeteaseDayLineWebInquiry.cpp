#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include "NeteaseDayLineWebInquiry.h"

CNeteaseDayLineWebInquiry::CNeteaseDayLineWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("http://quotes.money.163.com/service/chddata.html?code=");
  m_strWebDataInquireSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
  m_strConnection = _T("NeteaseDayLine");
}

CNeteaseDayLineWebInquiry::~CNeteaseDayLineWebInquiry() {
}

/////////////////////////////////////////////////////////////////////////////////
//
// 查询字符串的格式为：
//
//
////////////////////////////////////////////////////////////////////////////////
void CNeteaseDayLineWebInquiry::InquireNextWebData(void) {
  CString strMiddle = _T("");
  char buffer2[200];

  // 准备网易日线数据申请格式
  strMiddle = gl_ChinaStockMarket.CreateNeteaseDayLineInquiringStr();
  if (strMiddle.GetLength() > 0) {
    SetDownLoadingStockCode(strMiddle);
    gl_ChinaStockMarket.SetStockCodeForInquiringNeteaseDayLine(strMiddle);
    strMiddle += _T("&start=19900101&end=");
    sprintf_s(buffer2, "%8d", gl_ChinaStockMarket.GetDay());
    strMiddle += buffer2;
    CreateTotalInquiringString(strMiddle);
    gl_ChinaStockMarket.CheckValidOfNeteaseDayLineInquiringStr(strMiddle);
    SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
    StartReadingThread();
  }
}

void CNeteaseDayLineWebInquiry::StartReadingThread(void) {
  AfxBeginThread(ThreadReadNeteaseDayLine, (LPVOID)this);
}

void CNeteaseDayLineWebInquiry::SetDownLoadingStockCode(CString strStockCode) {
  char* p = strStockCode.GetBuffer();
  char cFirstChar = *p;
  CString strRight = strStockCode.Right(6);
  if (cFirstChar == '0') {
    m_strDownLoadingStockCode = _T("sh");
    m_strDownLoadingStockCode += strRight;
  }
  else if (cFirstChar == '1') {
    m_strDownLoadingStockCode = _T("sz");
    m_strDownLoadingStockCode += strRight;
  }
  else {
    m_strDownLoadingStockCode = strStockCode;
  }
}