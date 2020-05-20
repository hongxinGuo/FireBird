#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include "SinaRTWebInquiry.h"

using namespace std;
#include<thread>

CSinaRTWebInquiry::CSinaRTWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("http://hq.sinajs.cn/list=");
  m_strWebDataInquireSuffix = _T("");
  m_strConnection = _T("SinaRT");
  m_lInquiringNumber = 850; // 新浪实时数据查询数量默认值
}

CSinaRTWebInquiry::~CSinaRTWebInquiry() {
}

bool CSinaRTWebInquiry::PrepareNextInquiringStr(void) {
  CRTDataPtr pRTData = nullptr;

  CString strMiddle = _T("");

  // 申请下一批次股票实时数据
  if (gl_pChinaStockMarket->IsCheckActiveStock() || !gl_pChinaStockMarket->IsSystemReady()) { // 如果处于寻找今日活跃股票期间（9:10--9:29, 11:31--12:59),则使用全局股票池
    strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, false);
  }
  else { // 开市时使用今日活跃股票池
    strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, true);
  }
  gl_pChinaStockMarket->SetStockCodeForInquiringRTData(strMiddle.Left(8)); // 只提取第一个股票代码。新浪代码格式为：sh600000，共八个字符。
  CreateTotalInquiringString(strMiddle);

  return true;
}

CString CSinaRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
  return gl_pChinaStockMarket->GetSinaInquiringStockStr(lTotalNumber, fSkipUnactiveStock);
}

void CSinaRTWebInquiry::StartReadingThread(void) {
  RunningThreadReadSinaRTData();
}

bool CSinaRTWebInquiry::RunningThreadReadSinaRTData(void) {
  thread thread1(ThreadReadSinaRTData, this);
  thread1.detach();
  return true;
}

bool CSinaRTWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("读入%d个新浪实时数据\n", lNumberOfData);
  return true;
}