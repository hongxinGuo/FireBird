#include"pch.h"
#include"globedef.h"
#include"Thread.h"

#include "FinnhubWebInquiry.h"
#include"WorldMarket.h"

using namespace std;
#include<thread>

//
//m_strWebDataInquireSuffix = _T("&token=bv4ac1n48v6tcp17l5cg"); // 密钥放在最后
//m_strWebDataInquireSuffix = _T("&token=sandbox_bv8t3mn48v6rnm7c6aug"); // sandbox的密钥.sandbox只能用于测试，其数据为伪数据。
// 下面的是第二个,用于dell240工作机。
//m_strWebDataInquireSuffix = _T("&token=bv985d748v6ujthqfke0"); // 密钥放在最后
//m_strWebDataInquireSuffix = _T("&token=sandbox_bv985d748v6ujthqfkeg"); // sandbox的密钥.sandbox只能用于测试，其数据为伪数据。

CFinnhubWebInquiry::CFinnhubWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T(""); // finnhub有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。

  // finnhub不允许一个账户同时用于两个机器上，故而使用两个账户，看看能否避开速度限制。
  // 有可能一个网址只允许一个账户运行，则这种两个账户的方法也不起作用。
  if (m_strWebDataInquireSuffix.GetLength() < 5) {
#ifdef DEBUG
    m_strWebDataInquireSuffix = _T("&token=bv4ac1n48v6tcp17l5cg"); // 调试版使用ymail账户
    //m_strWebDataInquireSuffix = _T("&token=sandbox_bv8t3mn48v6rnm7c6aug"); // sandbox的密钥.sandbox只能用于测试，其数据为伪数据。
#else
    //m_strWebDataInquireSuffix = _T("&token=bv4ac1n48v6tcp17l5cg"); // 调试版使用ymail账户
    m_strWebDataInquireSuffix = _T("&token=bv985d748v6ujthqfke0"); // 发行版使用hotmail账户
  //m_strWebDataInquireSuffix = _T("&token=sandbox_bv985d748v6ujthqfkeg"); // sandbox的密钥.sandbox只能用于测试，其数据为伪数据。
#endif // DEBUG
  }
  m_strConnectionName = _T("Finnhub");
  m_lInquiringNumber = 1; // Finnhub实时数据查询数量默认值
}

CFinnhubWebInquiry::~CFinnhubWebInquiry() {
}

bool CFinnhubWebInquiry::PrepareNextInquiringStr(void) {
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
CString CFinnhubWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
  CString str = _T("");
  static int s_iCount = 5;

  switch (gl_pWorldMarket->GetCurrentFinnhubPrefixIndex()) {
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

void CFinnhubWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadFinnhubData, this);
  thread1.detach();
}

bool CFinnhubWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("读入%d个新浪实时数据\n", lNumberOfData);
  return true;
}