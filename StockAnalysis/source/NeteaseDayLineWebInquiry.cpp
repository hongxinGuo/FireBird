#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include "NeteaseDayLineWebInquiry.h"

using namespace std;
#include<thread>

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
bool CNeteaseDayLineWebInquiry::PrepareNextInquiringStr(void) {
  CString strMiddle = _T("");
  char buffer2[200];

  // 准备网易日线数据申请格式
  strMiddle = gl_pChinaStockMarket->CreateNeteaseDayLineInquiringStr();
  if (strMiddle.GetLength() > 0) {
    SetDownLoadingStockCode(strMiddle);
    gl_pChinaStockMarket->SetStockCodeForInquiringNeteaseDayLine(strMiddle);
    strMiddle += _T("&start=19900101&end=");
    sprintf_s(buffer2, "%8d", gl_pChinaStockMarket->GetDay());
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