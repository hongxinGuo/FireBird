#include"stdafx.h"

#include"globedef.h"

#include "VirtualMarket.h"

CVirtualMarket::CVirtualMarket(void) {
  m_fPermitResetMarket = true; // 允许系统被重置标识，唯独此标识不允许系统重置。初始时设置为真：允许重置系统。
  m_fResetMarket = true;
  m_fReadyToRun = true;

  m_lMarketLastTradeDay = 0;
  m_lMarketTime = 0;
  m_lMarketToday = 0;
  m_tLocal = 0;
  m_tMarket = 0;

  m_lTimeZoneOffset = -8 * 3600; // 本系统默认标准时间为东八区（北京标准时间）。
}

CVirtualMarket::~CVirtualMarket(void) {
}

#ifdef _DEBUG
void CVirtualMarket::AssertValid() const {
  CObject::AssertValid();
}

void CVirtualMarket::Dump(CDumpContext& dc) const {
  CObject::Dump(dc);
}
#endif //_DEBUG

bool CVirtualMarket::SchedulingTask(void) {
  static time_t s_timeLast = 0;
  CalculateTime();
  //根据时间，调度各项定时任务.每秒调度一次
  if (GetLocalTime() > s_timeLast) {
    SchedulingTaskPerSecond(GetLocalTime() - s_timeLast);
    s_timeLast = GetLocalTime();
  }
  return true;
}

void CVirtualMarket::CalculateTime(void) {
  time(&m_tLocal);
  m_tMarket = m_tLocal - m_lTimeZoneOffset;
  gmtime_s(&m_tmMarket, &m_tMarket);
  m_lMarketToday = (m_tmMarket.tm_year + 1900) * 10000 + (m_tmMarket.tm_mon + 1) * 100 + m_tmMarket.tm_mday;
  m_lMarketTime = m_tmMarket.tm_hour * 10000 + m_tmMarket.tm_min * 100 + m_tmMarket.tm_sec;
}

void CVirtualMarket::CalculateLastTradeDay(void) {
  time_t ttime = 0;

  switch (m_tmMarket.tm_wday) {
  case 1: // 星期一
  ttime = m_tMarket - 3 * 24 * 3600; //
  break;
  case 0: //星期日
  ttime = m_tMarket - 3 * 24 * 3600; //
  break;
  case 6: // 星期六
  ttime = m_tMarket - 2 * 24 * 3600; //
  break;
  default: // 其他
  ttime = m_tMarket - 24 * 3600; //
  }
  tm tm_;
  gmtime_s(&tm_, &ttime);
  m_lMarketLastTradeDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
}

bool CVirtualMarket::IsWorkingDay(void) {
  if ((m_tmMarket.tm_wday == 0) || (m_tmMarket.tm_wday == 6)) {
    return false;
  }
  else return true;
}

bool CVirtualMarket::IsWorkingDay(CTime timeCurrent) {
  if ((timeCurrent.GetDayOfWeek() == 1) || (timeCurrent.GetDayOfWeek() == 7)) {
    return false;
  }
  else return true;
}

bool CVirtualMarket::IsWorkingDay(long lDay) {
  const long year = lDay / 10000;
  const long month = lDay / 100 - year * 100;
  const long day = lDay - year * 10000 - month * 100;
  CTime ct(year, month, day, 12, 0, 0);

  if ((ct.GetDayOfWeek() == 1) || (ct.GetDayOfWeek() == 7)) {
    return false;
  }
  else return true;
}

bool CVirtualMarket::IsEarlyThen(long lEarlyDay, long lLatelyDay, long lTimeSpawnOfDays) {
  CTimeSpan ts(lTimeSpawnOfDays, 0, 0, 0);
  const long year = lEarlyDay / 10000;
  const long month = lEarlyDay / 100 - year * 100;
  const long day = lEarlyDay - year * 10000 - month * 100;
  CTime ctEarly(year, month, day, 12, 0, 0);
  ctEarly += ts;
  long lNewDay = ctEarly.GetYear() * 10000 + ctEarly.GetMonth() * 100 + ctEarly.GetDay();
  return (lNewDay < lLatelyDay);
}

long CVirtualMarket::GetNextDay(long lDay, long lTimeSpanDays) {
  CTimeSpan ts(lTimeSpanDays, 0, 0, 0);
  const long year = lDay / 10000;
  const long month = lDay / 100 - year * 100;
  const long day = lDay - year * 10000 - month * 100;
  CTime ctDay(year, month, day, 12, 0, 0);
  ctDay += ts;
  long lNewDay = ctDay.GetYear() * 10000 + ctDay.GetMonth() * 100 + ctDay.GetDay();
  return (lNewDay);
}

CString CVirtualMarket::GetTimeString(void) {
  char buffer[30];
  tm tmLocal;

  localtime_s(&tmLocal, &m_tLocal);
  sprintf_s(buffer, "%02d:%02d:%02d ", tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
  CString str;
  str = buffer;
  return(str);
}

bool CVirtualMarket::SchedulingTaskPerSecond(long lSecond) {
  const long lCurrentTime = GetTime();
  //long lCurrentTime2 = GetTime();

  // 各调度程序按间隔时间大小顺序排列，间隔时间长的必须位于间隔时间短的之前。
  SchedulingTaskPerHour(lSecond, lCurrentTime);
  SchedulingTaskPer5Minutes(lSecond, lCurrentTime);
  SchedulingTaskPer1Minute(lSecond, lCurrentTime);
  SchedulingTaskPer10Seconds(lSecond, lCurrentTime);

  return true;
}

bool CVirtualMarket::SchedulingTaskPer10Seconds(long lSecond, long lCurrentTime) {
  return true;
}

bool CVirtualMarket::SchedulingTaskPer1Minute(long lSecond, long lCurrentTime) {
  static int i1MinuteCounter = 59;  // 一分钟一次的计数器

 // 计算每分钟一次的任务。所有的定时任务，要按照时间间隔从长到短排列，即现执行每分钟一次的任务，再执行每秒钟一次的任务，这样能够保证长间隔的任务优先执行。
  if (i1MinuteCounter <= 0) {
    i1MinuteCounter = 59; // 重置计数器
    ResetMarketFlagAtMidnight(lCurrentTime);
  }
  return true;
}

void CVirtualMarket::ResetMarketFlagAtMidnight(long lCurrentTime) {
  // 午夜过后重置各种标识
  if (lCurrentTime <= 1500 && !IsPermitResetMarket()) {  // 在零点到零点十五分，重置系统标识
    m_fPermitResetMarket = true;
    CString str;
    str = _T("重置系统重置标识");
    gl_systemMessage.PushInformationMessage(str);
  }
}

bool CVirtualMarket::SchedulingTaskPer5Minutes(long lSecond, long lCurrentTime) {
  return true;
}

bool CVirtualMarket::SchedulingTaskPerHour(long lSecond, long lCurrentTime) {
  return true;
}