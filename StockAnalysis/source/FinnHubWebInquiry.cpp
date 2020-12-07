#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"

#include "FinnhubWebInquiry.h"
#include"AmericaStakeMarket.h"

using namespace std;
#include<thread>

CFinnhubWebInquiry::CFinnhubWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("https://finnhub.io/api/v1/quote?symbol=");
  m_strWebDataInquireSuffix = _T("&token=bv4ac1n48v6tcp17l5cg"); // 密钥放在最后
  //m_strWebDataInquireSuffix = _T(""); // 密钥放在最后
  m_strConnection = _T("FinnHubRT");
  m_lInquiringNumber = 1; // FinnHub实时数据查询数量默认值
}

CFinnhubWebInquiry::~CFinnhubWebInquiry() {
}

bool CFinnhubWebInquiry::PrepareNextInquiringStr(void) {
  CWebRTDataPtr pRTData = nullptr;

  CString strMiddle = _T("");

  // 申请下一批次股票实时数据。 此网络数据提取器使用FinnHubMarket
  // 由于Finnhub提供各种数据，而每个数据分别设计提取器会导致出现太多的提取器，故而在此分类。

  // 1 准备前缀字符串
  // 2. 准备中间字符串
  // 3. 准备后缀字符串
  //
  CreateTotalInquiringString(m_strWebDataInquireMiddle);

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CFinnhubWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
  CString str;
  static int s_iCount = 5;

  switch (gl_pAmericaStakeMarket->GetCurrentPrefixIndex()) {
  case __COMPANY_SYMBOLS__:
  // 无需中间串
  break;
  case __QUOTE__:
  break;
  default:
  break;
  }

  return str;
}

void CFinnhubWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadFinnHubData, this);
  thread1.detach();
}

bool CFinnhubWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("读入%d个新浪实时数据\n", lNumberOfData);
  return true;
}