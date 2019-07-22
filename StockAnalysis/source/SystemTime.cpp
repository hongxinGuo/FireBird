#include "stdafx.h"

#include"globedef.h"

#include "SystemTime.h"
#include"SystemMessage.h"

CSystemTime::CSystemTime(void)
{
  static int siInstance = 0;
  if (siInstance++ > 0) {
    TRACE("ϵͳʱ��ֻ��������һ��ʵ��\n");
    gl_systemMessage.PushInformationMessage(_T("ϵͳʱ��ֻ��������һ��ʵ��"));
  }

  m_lLastTradeDay = 0;
  m_lTime = 0;
  m_lToday = 0;
  m_ttime = 0;
  m_tm.tm_hour = m_tm.tm_mday = m_tm.tm_min = m_tm.tm_mon = m_tm.tm_sec
    = m_tm.tm_wday = m_tm.tm_yday = m_tm.tm_year = 0;
}

CSystemTime::~CSystemTime(void)
{
}

void CSystemTime::Sett_time(time_t ttime) {
  m_ttime = ttime;
  localtime_s(&m_tm, &m_ttime);
  m_lToday = (m_tm.tm_year + 1900) * 10000 + (m_tm.tm_mon + 1) * 100 + m_tm.tm_mday;
  m_lTime = m_tm.tm_hour * 10000 + m_tm.tm_min * 100 + m_tm.tm_sec;
}
 
void CSystemTime::CalculateTime(void)
{
  time(&m_ttime);
  localtime_s(&m_tm, &m_ttime);
  m_lToday = (m_tm.tm_year + 1900) * 10000 + (m_tm.tm_mon + 1) * 100 + m_tm.tm_mday;
  m_lTime = m_tm.tm_hour * 10000 + m_tm.tm_min * 100 + m_tm.tm_sec;
}

void CSystemTime::CalculateLastTradeDay(void)
{
  time_t ttime = 0;

  switch (m_tm.tm_wday) {
  case 1: // ����һ
    ttime = m_ttime - 3 * 24 * 3600; // 
    break;
  case 0: //������
    ttime = m_ttime - 3 * 24 * 3600; // 
    break;
  case 6: // ������
    ttime = m_ttime - 2 * 24 * 3600; // 
    break;
  default: // ����
    ttime = m_ttime - 24 * 3600; // 
  }
  tm tm_;
  localtime_s(&tm_, &ttime);
  m_lLastTradeDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;

}

long CSystemTime::ChangeTimeToDay( time_t time ) {
	ASSERT( time >= 0 );
	CTime ct( time );
	long lDay = ct.GetYear() * 10000 + ct.GetMonth() * 100 + ct.GetDay();
	return ( lDay );
}

time_t CSystemTime::ChangeDayToMarketCloseTime( long lDay ) {
  ASSERT ( lDay > 19700000 );
  long lYear = lDay / 10000;
  long lMonth = (lDay - lYear * 10000)/100;
  long lD = (lDay - lYear * 10000 - lMonth * 100);
	CTime ct( lYear, lMonth, lD, 15, 0, 0 );	// ����ʱ��15ʱ��UTC7ʱ
  return ( ct.GetTime() );
}

CString CSystemTime::GetTimeString(void) {
  char buffer[30];
  sprintf_s(buffer, "%02d:%02d:%02d ", m_tm.tm_hour, m_tm.tm_min, m_tm.tm_sec);
  CString str;
  str = buffer;
  return(str);
}

