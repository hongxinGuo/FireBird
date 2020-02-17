#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include "SinaRTWebInquiry.h"

CSinaRTWebInquiry::CSinaRTWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("http://hq.sinajs.cn/list=");
  m_strWebDataInquireSuffix = _T("");
  m_strConnection = _T("SinaRT");
}

CSinaRTWebInquiry::~CSinaRTWebInquiry() {
}

bool CSinaRTWebInquiry::PrepareNextInquiringStr(void) {
  CRTDataPtr pRTData = nullptr;
  static int iTotalInquiringStocks = 0;
  const int iInquiringNumber = 850;

  CString strMiddle = _T("");

  // 申请下一批次股票实时数据
  if (gl_ChinaStockMarket.IsCheckActiveStock() || !gl_ChinaStockMarket.SystemReady()) { // 如果处于寻找今日活跃股票期间（9:10--9:29, 11:31--12:59),则使用全局股票池
    iTotalInquiringStocks += iInquiringNumber;
    strMiddle = GetNextInquiringMiddleStr(iInquiringNumber, false);
    if ((iTotalInquiringStocks > gl_ChinaStockMarket.GetTotalStock() * 3) && (gl_ChinaStockMarket.GetRTDataReceived() > 10000)) {
      if (!gl_ChinaStockMarket.SystemReady()) { // 如果系统尚未设置好，则显示系统准备
        gl_systemMessage.PushInformationMessage(_T("中国股票市场初始化完毕"));
      }
      gl_ChinaStockMarket.SetSystemReady(true); // 所有的股票实时数据都轮询三遍，当日活跃股票集已经建立，故而可以接受日线数据了。
    }
  }
  else { // 开市时使用今日活跃股票池
    strMiddle = GetNextInquiringMiddleStr(iInquiringNumber, true);
  }
  gl_ChinaStockMarket.SetStockCodeForInquiringSinaRTData(strMiddle.Left(8)); // 只提取第一个股票代码
  CreateTotalInquiringString(strMiddle);

  return true;
}

CString CSinaRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
  return gl_ChinaStockMarket.GetSinaInquiringStockStr(lTotalNumber, fSkipUnactiveStock);
}

void CSinaRTWebInquiry::StartReadingThread(void) {
  AfxBeginThread(ThreadReadSinaRTData, this);
}

bool CSinaRTWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("读入%d个新浪实时数据\n", lNumberOfData);
  return true;
}