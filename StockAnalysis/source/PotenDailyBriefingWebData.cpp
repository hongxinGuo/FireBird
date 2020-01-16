#include"stdafx.h"
#include"globedef.h"
#include"Thread.h"
#include"Market.h"

#include"Accessory.h"

#include "PotenDailyBriefingWebData.h"

CPotenDailyBriefingWebData::CPotenDailyBriefingWebData() : CWebData() {
  m_strWebDataInquirePrefix = _T("http://energy.poten.com/poten-daily-briefing-webpage-");
  m_strWebDataInquireSuffix = _T("");
  m_strConnection = _T("CPotenDailyBriefing");

  m_lInquiringDay = 20180411; //poten.com��վ�����¸�ʽ�Ӵ����ڿ�ʼ��֮ǰ�ĸ�ʽ��ʱ��ȥ��ȡ��
}

CPotenDailyBriefingWebData::~CPotenDailyBriefingWebData() {
}

void CPotenDailyBriefingWebData::InquireNextWebData(void) {
  CString strMiddle = _T("");
  char buffer[50];
  long year = m_lInquiringDay / 10000;
  long month = m_lInquiringDay / 100 - year * 100;
  long day = m_lInquiringDay - year * 10000 - month * 100;

  sprintf_s(buffer, _T("%02d/%02d/%04d"), day, month, year);
  strMiddle = buffer;
  CreateTotalInquiringString(strMiddle);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

void CPotenDailyBriefingWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadPotenDailyBriefing, this);
}