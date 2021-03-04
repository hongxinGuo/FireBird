#include"pch.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include "NeteaseDayLineWebInquiry.h"

using namespace std;
#include<thread>

CNeteaseDayLineWebInquiry::CNeteaseDayLineWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("http://quotes.money.163.com/service/chddata.html?code=");
  m_strWebDataInquireSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
  m_strConnectionName = _T("NeteaseDayLine");
}

CNeteaseDayLineWebInquiry::~CNeteaseDayLineWebInquiry() {
}

/////////////////////////////////////////////////////////////////////////////////
//
// 查询字符串的格式为：
//
//
////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDayLineWebInquiry::PrepareNextInquiringStr(void) {
  CString strMiddle = _T("");
  char buffer2[200];
  CString strStockCode;

  // 准备网易日线数据申请格式
  if (gl_pChinaStockMarket->CreateNeteaseDayLineInquiringStr(strMiddle, gl_pChinaStockMarket->GetTotalStock())) {
    strStockCode = XferNeteaseToStandred(strMiddle);
    SetDownLoadingStockCode(strStockCode);
    gl_pChinaStockMarket->SetStockCodeForInquiringNeteaseDayLine(strStockCode);
    strMiddle += _T("&start=19900101&end=");
    sprintf_s(buffer2, _T("%8d"), gl_pChinaStockMarket->GetFormatedMarketDate());
    strMiddle += buffer2;
    CreateTotalInquiringString(strMiddle);
    gl_pChinaStockMarket->CheckValidOfNeteaseDayLineInquiringStr(strMiddle);

    return true;
  }
  return false;
}

void CNeteaseDayLineWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadNeteaseDayLine, this);
  thread1.detach();
}

/// <summary>
/// 这里的strStockCode为标准制式：600000.SS，000001.SZ，
/// </summary>
/// <param name="strStockCode"></param>
void CNeteaseDayLineWebInquiry::SetDownLoadingStockCode(CString strStockCode) {
  m_strDownLoadingStockCode = strStockCode;
}