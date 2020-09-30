#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include "NeteaseDayLineWebInquiry.h"

using namespace std;
#include<thread>

CNeteaseDLWebInquiry::CNeteaseDLWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("http://quotes.money.163.com/service/chddata.html?code=");
  m_strWebDataInquireSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
  m_strConnection = _T("NeteaseDL");
}

CNeteaseDLWebInquiry::~CNeteaseDLWebInquiry() {
}

/////////////////////////////////////////////////////////////////////////////////
//
// ��ѯ�ַ����ĸ�ʽΪ��
//
//
////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDLWebInquiry::PrepareNextInquiringStr(void) {
  CString strMiddle = _T("");
  char buffer2[200];

  // ׼�������������������ʽ
  if (gl_pChinaStockMarket->CreateNeteaseDLInquiringStr(strMiddle)) {
    SetDownLoadingStockCode(strMiddle);
    gl_pChinaStockMarket->SetStockCodeForInquiringNeteaseDL(strMiddle);
    strMiddle += _T("&start=19900101&end=");
    sprintf_s(buffer2, _T("%8d"), gl_pChinaStockMarket->GetFormatedMarketDate());
    strMiddle += buffer2;
    CreateTotalInquiringString(strMiddle);
    gl_pChinaStockMarket->CheckValidOfNeteaseDLInquiringStr(strMiddle);

    return true;
  }
  return false;
}

void CNeteaseDLWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadNeteaseDL, this);
  thread1.detach();
}

void CNeteaseDLWebInquiry::SetDownLoadingStockCode(CString strStockCode) {
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