#include"stdafx.h"
#include"globedef.h"
#include"ChinaMarket.h"

#include"Thread.h"
#include "TengxunWebRTData.h"

CTengxunRTWebData::CTengxunRTWebData() : CWebData() {
  m_strWebDataInquirePrefix = _T("http://qt.gtimg.cn/q=");
  m_strWebDataInquireSuffix = _T("");
  m_strConnection = _T("TengxunRT");
}

CTengxunRTWebData::~CTengxunRTWebData() {
}

void CTengxunRTWebData::InquireNextWebData(void) {
  CString strMiddle = _T("");
  ASSERT(gl_ChinaStockMarket.SystemReady());

  // 申请下一批次股票实时数据。
  // 申请腾讯实时数据时，如果遇到不存在的股票代码，服务器会返回v_pv_none_match="1";，导致系统故障，
  // 故而现在只使用有效股票代码。
  if (!gl_ChinaStockMarket.SystemReady()) { // 如果系统尚未准备好，则使用全局股票池
    strMiddle = GetNextInquiringStr(900, false);
  }
  else { // 开市时使用今日活跃股票池
    strMiddle = GetNextInquiringStr(900, false); // 目前暂时还是使用全部股票池
  }

  CreateTotalInquiringString(strMiddle);
  SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
  StartReadingThread();
}

CString CTengxunRTWebData::GetNextInquiringStr(long lTotalNumber, bool fSkipUnactiveStock) {
  return gl_ChinaStockMarket.GetTengxunInquiringStockStr(lTotalNumber, fSkipUnactiveStock);
}

void CTengxunRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadTengxunRTData, this);
}

bool CTengxunRTWebData::ReportStatus(long lNumberOfData) {
  TRACE("读入%d个腾讯实时数据\n", lNumberOfData);
  return true;
}