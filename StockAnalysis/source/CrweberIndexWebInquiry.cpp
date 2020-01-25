#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include"Accessory.h"

#include "CrweberIndexWebInquiry.h"

CCrweberIndexWebInquiry::CCrweberIndexWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("http://www.crweber.com");
  m_strWebDataInquireSuffix = _T("");
  m_strConnection = _T("CrweberIndex");
}

CCrweberIndexWebInquiry::~CCrweberIndexWebInquiry() {
}

void CCrweberIndexWebInquiry::InquireNextWebData(void) {
  CString strMiddle = _T("");

  CreateTotalInquiringString(strMiddle);
  SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
  StartReadingThread();
}

void CCrweberIndexWebInquiry::StartReadingThread(void) {
  AfxBeginThread(ThreadReadCrweberIndex, this);
}