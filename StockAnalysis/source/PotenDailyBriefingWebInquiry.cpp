#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include"Accessory.h"

#include "PotenDailyBriefingWebInquiry.h"

using namespace std;
#include<thread>

CPotenDailyBriefingWebInquiry::CPotenDailyBriefingWebInquiry() : CVirtualWebInquiry() {
  m_strWebDataInquirePrefix = _T("http://energy.poten.com/poten-daily-briefing-webpage-");
  m_strWebDataInquireSuffix = _T("");
  m_strConnection = _T("PotenDailyBriefing");

  m_lInquiringDate = 20180411; //poten.com��վ�����¸�ʽ�Ӵ�����֮��ʼ��֮ǰ�ĸ�ʽ��ʱ��ȥ��ȡ��
}

CPotenDailyBriefingWebInquiry::~CPotenDailyBriefingWebInquiry() {
}

bool CPotenDailyBriefingWebInquiry::PrepareNextInquiringStr(void) {
  m_lInquiringDate = gl_pPotenDailyBriefingMarket->GetCurrentInquiringDate();
  CString strMiddle = _T("");
  char buffer[50];
  long year = m_lInquiringDate / 10000;
  long month = m_lInquiringDate / 100 - year * 100;
  long day = m_lInquiringDate - year * 10000 - month * 100;

  sprintf_s(buffer, _T("%02d/%02d/%04d"), month, day, year);
  strMiddle = buffer;
  CreateTotalInquiringString(strMiddle);
  TRACE(_T("��ȡ%08d�յ�poten����\n"), m_lInquiringDate);
  return true;
}

void CPotenDailyBriefingWebInquiry::StartReadingThread(void) {
  thread thread1(ThreadReadPotenDailyBriefing, this);
  thread1.detach();
}