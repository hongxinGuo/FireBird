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

bool CCrweberIndexWebInquiry::PrepareNextInquiringStr(void) {
  CString strMiddle = _T("");

  CreateTotalInquiringString(strMiddle);
  return true;
}

void CCrweberIndexWebInquiry::StartReadingThread(void) {
  AfxBeginThread(ThreadReadCrweberIndex, this);
}