#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"Market.h"

#include"Accessory.h"

#include "CrweberIndexWebData.h"

CCrweberIndexWebData::CCrweberIndexWebData() : CWebData() {
  m_strWebDataInquirePrefix = _T("http://www.crweber.com");
  m_strWebDataInquireSuffix = _T("");
}

CCrweberIndexWebData::~CCrweberIndexWebData() {
}

void CCrweberIndexWebData::InquireNextWebData(void) {
  CString strMiddle = _T("");

  CreateTotalInquiringString(strMiddle);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

int CCrweberIndexWebData::GetInquiringStr(CString& strInquire, long, bool) {
  strInquire = _T("");
  return 0;
}

void CCrweberIndexWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadCrweberIndex, this);
}