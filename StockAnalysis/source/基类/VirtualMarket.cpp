#include"pch.h"

#include"globedef.h"

#include "VirtualMarket.h"

// 所有的市场使用同一个当地时间。
time_t CVirtualMarket::sm_tLocal = 0;

CVirtualMarket::CVirtualMarket(void) : CObject() {
  m_fPermitResetMarket = true; // 允许系统被重置标识，唯独此标识不允许系统重置。初始时设置为真：允许重置系统。
  m_fResetMarket = true;
  m_fReadyToRun = true;
  m_fSystemReady = true; // 默认为真

  m_lMarketLastTradeDate = 0;
  m_lMarketTime = 0;
  m_lMarketDate = 0;
  m_tMarket = 0;

  m_strMarketId = _T("Warning: CVirtualMarket Called.");
  m_lMarketTimeZone = -8 * 3600; // 本系统默认标准时间为东八区（北京标准时间）。

  m_i10SecondCounter = 9;  // 十秒一次的计数器
  m_i1MinuteCounter = 59;  // 一分钟一次的计数器
  m_i5MinuteCounter = 299;  // 五分钟一次的计数器
  m_i1HourCounter = 3599;  // 一小时一次的计数器
  m_timeLast = 0;
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
  CalculateTime();

  time_t timeDiffer = GetMarketTime() - m_timeLast;
  //根据时间，调度各项定时任务.每秒调度一次
  if (timeDiffer > 0) {
    SchedulingTaskPerSecond(timeDiffer);
    m_timeLast = GetMarketTime();
    return true;
  }
  else return false;
}

void CVirtualMarket::ResetMarket(void) {
  ASSERT(0); // 不允许调用基类重置市场函数。这里只是为了测试方便的原因才定义一个实现。
}

bool CVirtualMarket::UpdateMarketInfo(void) {
  // do nothing
  return true;
}

void CVirtualMarket::CalculateTime(void) noexcept {
  time(&sm_tLocal);
  m_tMarket = sm_tLocal - m_lMarketTimeZone;
  gmtime_s(&m_tmMarket, &m_tMarket);
  m_lMarketDate = (m_tmMarket.tm_year + 1900) * 10000 + (m_tmMarket.tm_mon + 1) * 100 + m_tmMarket.tm_mday;
  m_lMarketTime = m_tmMarket.tm_hour * 10000 + m_tmMarket.tm_min * 100 + m_tmMarket.tm_sec;
}

void CVirtualMarket::CalculateLastTradeDate(void) noexcept {
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
  m_lMarketLastTradeDate = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
}

bool CVirtualMarket::IsWorkingDay(void) noexcept {
  if ((m_tmMarket.tm_wday == 0) || (m_tmMarket.tm_wday == 6)) {
    return false;
  }
  else return true;
}

bool CVirtualMarket::IsWorkingDay(CTime timeCurrent) noexcept {
  if ((timeCurrent.GetDayOfWeek() == 1) || (timeCurrent.GetDayOfWeek() == 7)) {
    return false;
  }
  else return true;
}

bool CVirtualMarket::IsWorkingDay(long lDate) noexcept {
  const long year = lDate / 10000;
  const long month = lDate / 100 - year * 100;
  const long day = lDate - year * 10000 - month * 100;
  CTime ct(year, month, day, 12, 0, 0);

  if ((ct.GetDayOfWeek() == 1) || (ct.GetDayOfWeek() == 7)) {
    return false;
  }
  else return true;
}

bool CVirtualMarket::IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays) noexcept {
  CTimeSpan ts(lTimeSpawnOfDays, 0, 0, 0);
  const long year = lEarlyDate / 10000;
  const long month = lEarlyDate / 100 - year * 100;
  const long day = lEarlyDate - year * 10000 - month * 100;
  CTime ctEarly(year, month, day, 12, 0, 0);
  ctEarly += ts;
  long lNewDate = ctEarly.GetYear() * 10000 + ctEarly.GetMonth() * 100 + ctEarly.GetDay();
  return (lNewDate < lLatelyDate);
}

long CVirtualMarket::GetNextDay(long lDate, long lTimeSpanDays) noexcept {
  CTimeSpan ts(lTimeSpanDays, 0, 0, 0);
  const long year = lDate / 10000;
  const long month = lDate / 100 - year * 100;
  const long day = lDate - year * 10000 - month * 100;
  CTime ctDay(year, month, day, 12, 0, 0);
  ctDay += ts;
  long lNewDate = ctDay.GetYear() * 10000 + ctDay.GetMonth() * 100 + ctDay.GetDay();
  return (lNewDate);
}

long CVirtualMarket::GetPrevDay(long lDate, long lTimeSpanDays) noexcept {
  CTimeSpan ts(lTimeSpanDays, 0, 0, 0);
  const long year = lDate / 10000;
  const long month = lDate / 100 - year * 100;
  const long day = lDate - year * 10000 - month * 100;
  CTime ctDay(year, month, day, 12, 0, 0);
  ctDay -= ts;
  long lNewDate = ctDay.GetYear() * 10000 + ctDay.GetMonth() * 100 + ctDay.GetDay();
  return (lNewDate);
}

CString CVirtualMarket::GetStringOfLocalTime(void) {
  char buffer[30];
  tm tmLocal;

  localtime_s(&tmLocal, &sm_tLocal);
  sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
  CString str;
  str = buffer;
  return(str);
}

CString CVirtualMarket::GetStringOfLocalDateTime(void) {
  char buffer[100];
  tm tmLocal;

  localtime_s(&tmLocal, &sm_tLocal);
  sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
  CString str;
  str = buffer;
  return(str);
}

CString CVirtualMarket::GetStringOfMarketTime(void) {
  char buffer[30];
  tm tmMarket;

  gmtime_s(&tmMarket, &m_tMarket);
  sprintf_s(buffer, _T("%02d:%02d:%02d "), tmMarket.tm_hour, tmMarket.tm_min, tmMarket.tm_sec);
  CString str;
  str = buffer;
  return(str);
}

CString CVirtualMarket::GetStringOfMarketDateTime(void) {
  char buffer[100];
  tm tmMarket;

  gmtime_s(&tmMarket, &m_tMarket);
  sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), tmMarket.tm_year + 1900, tmMarket.tm_mon + 1, tmMarket.tm_mday, tmMarket.tm_hour, tmMarket.tm_min, tmMarket.tm_sec);
  CString str;
  str = buffer;
  return(str);
}

CString CVirtualMarket::GetStringOfDate(long lDate) {
  char buffer[30];
  long year = lDate / 10000;
  long month = lDate / 100 - year * 100;
  long day = lDate - year * 10000 - month * 100;

  sprintf_s(buffer, _T("%4d年%2d月%2d日"), year, month, day);
  CString str;
  str = buffer;
  return(str);
}

CString CVirtualMarket::GetStringOfMarketDate(void) {
  return GetStringOfDate(m_lMarketDate);
}

void CVirtualMarket::TaskResetMarketFlagAtMidnight(long lCurrentTime) {
  // 午夜过后重置各种标识
  if (lCurrentTime <= 1500 && !IsPermitResetMarket()) {  // 在零点到零点十五分，重置系统标识
    m_fPermitResetMarket = true;
    CString str;
    str = m_strMarketId + _T("重置系统重置标识");
    TRACE(_T("%S \n"), str.GetBuffer());
    gl_systemMessage.PushInformationMessage(str);
  }
}

bool CVirtualMarket::SchedulingTaskPerSecond(long lSecond) {
  const long lCurrentTime = GetFormatedMarketTime();
  //long lCurrentTime2 = GetFormatedMarketTime();

  // 各调度程序按间隔时间大小顺序排列，间隔时间长的必须位于间隔时间短的之前。
  SchedulingTaskPerMinute(lSecond, lCurrentTime);

  return true;
}

bool CVirtualMarket::SchedulingTaskPerMinute(long lSecond, long lCurrentTime) {
  // 计算每分钟一次的任务。所有的定时任务，要按照时间间隔从长到短排列，即现执行每分钟一次的任务，再执行每秒钟一次的任务，这样能够保证长间隔的任务优先执行。
  m_i1MinuteCounter -= lSecond;
  if (m_i1MinuteCounter < 0) {
    m_i1MinuteCounter = 59; // 重置计数器
    TaskResetMarketFlagAtMidnight(lCurrentTime);
    return true;
  }
  else {
    return false;
  }
}