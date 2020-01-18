#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include"Accessory.h"

#include "PotenDailyBriefingWebData.h"

CPotenDailyBriefingWebData::CPotenDailyBriefingWebData() : CWebData() {
  m_strWebDataInquirePrefix = _T("http://energy.poten.com/poten-daily-briefing-webpage-");
  m_strWebDataInquireSuffix = _T("");
  m_strConnection = _T("PotenDailyBriefing");

  m_lInquiringDay = 20180411; //poten.com网站的最新格式从此日期开始，之前的格式暂时不去读取。
  m_lNextInquiringDay = 20180412;
}

CPotenDailyBriefingWebData::~CPotenDailyBriefingWebData() {
}

void CPotenDailyBriefingWebData::InquireNextWebData(void) {
  m_lInquiringDay = gl_PotenDailyBriefingMarket.GetNewestUpdateDay();
  CString strMiddle = _T("");
  char buffer[50];
  long year = m_lInquiringDay / 10000;
  long month = m_lInquiringDay / 100 - year * 100;
  long day = m_lInquiringDay - year * 10000 - month * 100;
  CTime today(year, month, day, 12, 0, 0);
  const CTimeSpan oneDay(1, 0, 0, 0);

  sprintf_s(buffer, _T("%02d/%02d/%04d"), month, day, year);
  strMiddle = buffer;
  CreateTotalInquiringString(strMiddle);
  SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
  StartReadingThread();
  TRACE(_T("读取%08d日的poten数据\n"), m_lInquiringDay);

  today += oneDay;
  m_lNextInquiringDay = today.GetYear() * 10000 + today.GetMonth() * 100 + today.GetDay();
  gl_PotenDailyBriefingMarket.SetNewestUpdateDay(m_lNextInquiringDay);
}

void CPotenDailyBriefingWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadPotenDailyBriefing, this);
}