#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"Market.h"

#include "NeteaseWebDayLineData.h"

CNeteaseWebDayLineData::CNeteaseWebDayLineData() : CWebData() {
  m_fNeedProcessingCurrentWebData = true;

  m_strWebDataInquirePrefix = _T("http://quotes.money.163.com/service/chddata.html?code=");
  m_strWebDataInquireSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
}

CNeteaseWebDayLineData::~CNeteaseWebDayLineData() {
}

bool CNeteaseWebDayLineData::GetWebData(void) {
  if (!IsReadingWebData()) {
    InquireNextWebData();
  }
  return true;
}

void CNeteaseWebDayLineData::InquireNextWebData(void) {
  CString strMiddle = _T("");
  char buffer2[200];

  // 准备网易日线数据申请格式
  m_fNeedProcessingCurrentWebData = gl_ChinaStockMarket.CreateNeteaseDayLineInquiringStr(strMiddle);
  if (m_fNeedProcessingCurrentWebData) {
    SetDownLoadingStockCode(strMiddle);
    strMiddle += _T("&start=19900101");
    strMiddle += _T("&end=");
    sprintf_s(buffer2, "%8d", gl_systemTime.GetDay());
    strMiddle += buffer2;
    CreateTotalInquiringString(strMiddle);
    SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
    StartReadingThread();
  }
}

int CNeteaseWebDayLineData::GetInquiringStr(CString& strInquire, long, bool) {
  strInquire = _T("");
  return 0;
}

void CNeteaseWebDayLineData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadNeteaseDayLine, (LPVOID)this);
}

void CNeteaseWebDayLineData::SetDownLoadingStockCode(CString strStockCode) {
  CString str = strStockCode.Left(1);
  CString strRight = strStockCode.Right(6);
  if (str.Compare(_T("0")) == 0) {
    m_strDownLoadingStockCode = _T("sh");
    m_strDownLoadingStockCode += strRight;
  }
  else if (str.Compare(_T("1")) == 0) {
    m_strDownLoadingStockCode = _T("sz");
    m_strDownLoadingStockCode += strRight;
  }
  else {
    m_strDownLoadingStockCode = strStockCode;
  }
}