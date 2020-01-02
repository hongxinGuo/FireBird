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

/////////////////////////////////////////////////////////////////////////////////
//
// ��ѯ�ַ����ĸ�ʽΪ��
//
//
////////////////////////////////////////////////////////////////////////////////
void CNeteaseWebDayLineData::InquireNextWebData(void) {
  CString strMiddle = _T("");
  char buffer2[200];

  // ׼�������������������ʽ
  m_fNeedProcessingCurrentWebData = gl_ChinaStockMarket.CreateNeteaseDayLineInquiringStr(strMiddle);
  if (m_fNeedProcessingCurrentWebData) {
    SetDownLoadingStockCode(strMiddle);
    strMiddle += _T("&start=19900101");
    strMiddle += _T("&end=");
    sprintf_s(buffer2, "%8d", gl_systemTime.GetDay());
    strMiddle += buffer2;
    CreateTotalInquiringString(strMiddle);
    SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
    StartReadingThread();
  }
}

void CNeteaseWebDayLineData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadNeteaseDayLine, (LPVOID)this);
}

void CNeteaseWebDayLineData::SetDownLoadingStockCode(CString strStockCode) {
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