#include "stdafx.h"

#include"globedef.h"

#include "SystemTime.h"
#include"SystemMessage.h"

CSystemTime::CSystemTime(void) {
  static int siInstance = 0;
  if (siInstance++ > 0) {
    TRACE("系统时间只允许生成一个实例\n");
    gl_systemMessage.PushInformationMessage(_T("系统时间只允许生成一个实例"));
  }

  m_lLocalLastTradeDay = 0;
  m_lLocalTime = 0;
  m_lLocalToday = 0;
  m_ttime = 0;
  m_tmLocal.tm_hour = m_tmLocal.tm_mday = m_tmLocal.tm_min = m_tmLocal.tm_mon = m_tmLocal.tm_sec
    = m_tmLocal.tm_wday = m_tmLocal.tm_yday = m_tmLocal.tm_year = 0;

  // 更新时间
  CalculateLocalTime();
}

CSystemTime::~CSystemTime(void) {
}

void CSystemTime::__Test_Sett_time(time_t ttime) {
  m_ttime = ttime;
  localtime_s(&m_tmLocal, &m_ttime);
  m_lLocalToday = (m_tmLocal.tm_year + 1900) * 10000 + (m_tmLocal.tm_mon + 1) * 100 + m_tmLocal.tm_mday;
  m_lLocalTime = m_tmLocal.tm_hour * 10000 + m_tmLocal.tm_min * 100 + m_tmLocal.tm_sec;
}

void CSystemTime::__Test_SetTM(tm tm_) {
  m_tmLocal = tm_;
}

void CSystemTime::CalculateLocalTime(void) {
  time(&m_ttime);
  localtime_s(&m_tmLocal, &m_ttime);
  m_lLocalToday = (m_tmLocal.tm_year + 1900) * 10000 + (m_tmLocal.tm_mon + 1) * 100 + m_tmLocal.tm_mday;
  m_lLocalTime = m_tmLocal.tm_hour * 10000 + m_tmLocal.tm_min * 100 + m_tmLocal.tm_sec;
}

void CSystemTime::CalculateGMTTime(void) {
  time(&m_ttime);
  gmtime_s(&m_tmGMT, &m_ttime);
  m_lGMTToday = (m_tmGMT.tm_year + 1900) * 10000 + (m_tmGMT.tm_mon + 1) * 100 + m_tmGMT.tm_mday;
  m_lGMTTime = m_tmGMT.tm_hour * 10000 + m_tmGMT.tm_min * 100 + m_tmGMT.tm_sec;
}

void CSystemTime::CalculateLastTradeDay(void) {
  time_t ttime = 0;

  switch (m_tmLocal.tm_wday) {
  case 1: // 星期一
  ttime = m_ttime - 3 * 24 * 3600; //
  break;
  case 0: //星期日
  ttime = m_ttime - 3 * 24 * 3600; //
  break;
  case 6: // 星期六
  ttime = m_ttime - 2 * 24 * 3600; //
  break;
  default: // 其他
  ttime = m_ttime - 24 * 3600; //
  }
  tm tm_;
  localtime_s(&tm_, &ttime);
  m_lLocalLastTradeDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
}

CString CSystemTime::GetTimeString(void) {
  char buffer[30];
  sprintf_s(buffer, "%02d:%02d:%02d ", m_tmLocal.tm_hour, m_tmLocal.tm_min, m_tmLocal.tm_sec);
  CString str;
  str = buffer;
  return(str);
}

bool CSystemTime::IsWorkingDay(void) {
  if ((m_tmLocal.tm_wday == 0) || (m_tmLocal.tm_wday == 6)) {
    return false;
  }
  else return true;
}

bool CSystemTime::IsWorkingDay(CTime timeCurrent) {
  if ((timeCurrent.GetDayOfWeek() == 1) || (timeCurrent.GetDayOfWeek() == 7)) {
    return false;
  }
  else return true;
}

bool CSystemTime::IsWorkingDay(long lDay) {
  const long year = lDay / 10000;
  const long month = lDay / 100 - year * 100;
  const long day = lDay - year * 10000 - month * 100;
  CTime ct(year, month, day, 12, 0, 0);

  if ((ct.GetDayOfWeek() == 1) || (ct.GetDayOfWeek() == 7)) {
    return false;
  }
  else return true;
}

bool CSystemTime::IsEarlyThen(long lEarlyDay, long lLatelyDay, long lTimeSpawnOfDays) {
  CTimeSpan ts(lTimeSpawnOfDays, 0, 0, 0);
  const long year = lEarlyDay / 10000;
  const long month = lEarlyDay / 100 - year * 100;
  const long day = lEarlyDay - year * 10000 - month * 100;
  CTime ctEarly(year, month, day, 12, 0, 0);
  ctEarly += ts;
  long lNewDay = ctEarly.GetYear() * 10000 + ctEarly.GetMonth() * 100 + ctEarly.GetDay();
  return (lNewDay < lLatelyDay);
}

long CSystemTime::GetNextDay(long lDay, long lTimeSpanDays) {
  CTimeSpan ts(lTimeSpanDays, 0, 0, 0);
  const long year = lDay / 10000;
  const long month = lDay / 100 - year * 100;
  const long day = lDay - year * 10000 - month * 100;
  CTime ctDay(year, month, day, 12, 0, 0);
  ctDay += ts;
  long lNewDay = ctDay.GetYear() * 10000 + ctDay.GetMonth() * 100 + ctDay.GetDay();
  return (lNewDay);
}