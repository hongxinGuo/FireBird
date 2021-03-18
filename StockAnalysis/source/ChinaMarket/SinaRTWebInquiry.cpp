#include"pch.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include "SinaRTWebInquiry.h"

using namespace std;
#include<thread>

CSinaRTWebInquiry::CSinaRTWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("http://hq.sinajs.cn/list=");
  m_strWebDataInquireSuffix = _T("");
  m_strConnectionName = _T("SinaRT");
  m_lInquiringNumber = 850; // 新浪实时数据查询数量默认值
}

CSinaRTWebInquiry::~CSinaRTWebInquiry() {
}

bool CSinaRTWebInquiry::PrepareNextInquiringStr(void) {
  CString strMiddle = _T("");
  CString strSinaStockCode;

  // 申请下一批次股票实时数据
  // 如果处于寻找今日活跃股票期间（9:10--9:29, 11:31--12:59),则使用全局股票池
  // 开市时使用今日活跃股票池
  strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, gl_pChinaStockMarket->IsCheckActiveStock());
  strSinaStockCode = strMiddle.Left(8); // 只提取第一个股票代码。新浪代码格式为：sh000001，共八个字符。
  gl_pChinaStockMarket->SetStockCodeForInquiringRTData(XferSinaToStandred(strSinaStockCode));
  CreateTotalInquiringString(strMiddle);

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 新浪网络实时数据提取函数。
// 目前只提取前12000个股票的实时数据。
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CSinaRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fCheckActiveStock) {
  return gl_pChinaStockMarket->GetSinaStockInquiringStr(lTotalNumber, fCheckActiveStock);
}

void CSinaRTWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadSinaRTData, this);
  thread1.detach();
}

bool CSinaRTWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("读入%d个新浪实时数据\n", lNumberOfData);
  return true;
}