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

  m_lLastTradeDay = 0;
  m_lTime = 0;
  m_lToday = 0;
  m_ttime = 0;
  m_tm.tm_hour = m_tm.tm_mday = m_tm.tm_min = m_tm.tm_mon = m_tm.tm_sec
    = m_tm.tm_wday = m_tm.tm_yday = m_tm.tm_year = 0;

  // 更新时间
  CalculateTime();
}

CSystemTime::~CSystemTime(void) noexcept {
}

void CSystemTime::Sett_time(time_t ttime) noexcept {
  m_ttime = ttime;
  localtime_s(&m_tm, &m_ttime);
  m_lToday = (m_tm.tm_year + 1900) * 10000 + (m_tm.tm_mon + 1) * 100 + m_tm.tm_mday;
  m_lTime = m_tm.tm_hour * 10000 + m_tm.tm_min * 100 + m_tm.tm_sec;
}

void CSystemTime::SetTime(long lTime) {
}

void CSystemTime::CalculateTime(void) noexcept {
  time(&m_ttime);
  localtime_s(&m_tm, &m_ttime);
  m_lToday = (m_tm.tm_year + 1900) * 10000 + (m_tm.tm_mon + 1) * 100 + m_tm.tm_mday;
  m_lTime = m_tm.tm_hour * 10000 + m_tm.tm_min * 100 + m_tm.tm_sec;
}

void CSystemTime::CalculateLastTradeDay(void) noexcept {
  time_t ttime = 0;

  switch (m_tm.tm_wday) {
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
  m_lLastTradeDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
}

time_t CSystemTime::ChangeDayToMarketCloseTime(long lDay) noexcept {
  ASSERT(lDay > 19700000);
  const long lYear = lDay / 10000;
  const long lMonth = (lDay - lYear * 10000) / 100;
  const long lD = (lDay - lYear * 10000 - lMonth * 100);
  CTime ct(lYear, lMonth, lD, 15, 0, 0);	// 北京时间15时即UTC7时
  return (ct.GetTime());
}

CString CSystemTime::GetTimeString(void) noexcept {
  char buffer[30];
  sprintf_s(buffer, "%02d:%02d:%02d ", m_tm.tm_hour, m_tm.tm_min, m_tm.tm_sec);
  CString str;
  str = buffer;
  return(str);
}

bool CSystemTime::IsWorkingDay(void)  noexcept {
  if ((m_tm.tm_wday == 0) || (m_tm.tm_wday == 6)) {
    return false;
  }
  else return true;
}

bool CSystemTime::IsWorkingDay(CTime timeCurrent) noexcept {
  if ((timeCurrent.GetDayOfWeek() == 1) || (timeCurrent.GetDayOfWeek() == 7)) {
    return false;
  }
  else return true;
}

long CSystemTime::FormatToDay(time_t const tt) noexcept {
  ASSERT(time >= 0);
  tm tm_;
  localtime_s(&tm_, &tt);
  return((tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday);
}

long CSystemTime::FormatToDay(tm* ptm) noexcept {
  return((ptm->tm_year + 1900) * 10000 + (ptm->tm_mon + 1) * 100 + ptm->tm_mday);
}