#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"

#include "QuandlWebInquiry.h"
#include"AmericaMarket.h"

using namespace std;
#include<thread>

//  api_key=zBMXMyoTyiy_N3pMb3ex
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // 密钥放在最后
// 下面的是第二个,用于dell240工作机。
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // 密钥放在最后

CQuandlWebInquiry::CQuandlWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T(""); // finnhub有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。

  // finnhub不允许一个账户同时用于两个机器上，故而使用两个账户，看看能否避开速度限制。
  // 有可能一个网址只允许一个账户运行，则这种两个账户的方法也不起作用。
  if (m_strWebDataInquireSuffix.GetLength() < 5) {
#ifdef DEBUG
    m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // 调试版使用ymail账户
#else
    m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // 发行版使用hotmail账户
#endif // DEBUG
  }
  m_strConnection = _T("Quandl");
  m_lInquiringNumber = 1; // Finnhub实时数据查询数量默认值
}

CQuandlWebInquiry::~CQuandlWebInquiry() {
}

bool CQuandlWebInquiry::PrepareNextInquiringStr(void) {
  CWebRTDataPtr pRTData = nullptr;

  CString strMiddle = _T("");

  // 申请下一批次股票实时数据。 此网络数据提取器使用FinnhubMarket
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
CString CQuandlWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
  CString str;
  static int s_iCount = 5;

  switch (gl_pAmericaMarket->GetCurrentPrefixIndex()) {
  case __COMPANY_SYMBOLS__:
  // 无需中间串
  break;
  case __STOCK_QUOTE__:
  break;
  default:
  break;
  }

  return str;
}

void CQuandlWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadQuandlData, this);
  thread1.detach();
}

bool CQuandlWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("读入%d个新浪实时数据\n", lNumberOfData);
  return true;
}