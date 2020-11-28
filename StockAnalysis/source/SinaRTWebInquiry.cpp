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
  CWebRTDataPtr pRTData = nullptr;

  CString strMiddle = _T("");

  // 申请下一批次股票实时数据
  if (gl_pChinaStakeMarket->IsCheckActiveStock() || !gl_pChinaStakeMarket->IsSystemReady()) { // 如果处于寻找今日活跃股票期间（9:10--9:29, 11:31--12:59),则使用全局股票池
    strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, false);
  }
  else { // 开市时使用今日活跃股票池
    strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, true);
  }
  gl_pChinaStakeMarket->SetStockCodeForInquiringRTData(strMiddle.Left(8)); // 只提取第一个股票代码。新浪代码格式为：sh600000，共八个字符。
  CreateTotalInquiringString(strMiddle);

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 新浪网络实时数据提取函数。
// 目前只提取前12000个股票的实时数据，准备加入提取其他证券实时数据的功能。
// 由于速度的限制，前12000个股票的实时数据提取必须达到每三秒一次，故而其他证券的查询只能利用空闲时间。
// 这里查询五次股票之后，再查询一次其他证券。
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CSinaRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
  CString str;
  static int s_iCount = 5;
  if (--s_iCount > 0) {// 提取前12000个股票的实时数据
    str = gl_pChinaStakeMarket->GetSinaStockInquiringStr(lTotalNumber, fSkipUnactiveStock);
  }
  else {// 提取后面其他证券的实时数据
    s_iCount = 5;
    str = gl_pChinaStakeMarket->GetSinaStakeInquiringStr(lTotalNumber, gl_pChinaStakeMarket->GetTotalStock(), fSkipUnactiveStock);
  }

  return str;
}

void CSinaRTWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadSinaRTData, this);
  thread1.detach();
}

bool CSinaRTWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("读入%d个新浪实时数据\n", lNumberOfData);
  return true;
}