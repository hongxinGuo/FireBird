#include"stdafx.h"

#include"globedef.h"

#include "VirtualMarket.h"

// ���е��г�ʹ��ͬһ������ʱ�䡣
time_t CVirtualMarket::sm_tLocal = 0;

CVirtualMarket::CVirtualMarket(void) {
  m_fPermitResetMarket = true; // ����ϵͳ�����ñ�ʶ��Ψ���˱�ʶ������ϵͳ���á���ʼʱ����Ϊ�棺��������ϵͳ��
  m_fResetMarket = true;
  m_fReadyToRun = true;

  m_lMarketLastTradeDay = 0;
  m_lMarketTime = 0;
  m_lMarketToday = 0;
  m_tMarket = 0;

  m_strMarketId = _T("Warning: CVirtualMarket Called.");
  m_lTimeZoneOffset = -8 * 3600; // ��ϵͳĬ�ϱ�׼ʱ��Ϊ��������������׼ʱ�䣩��

  m_i10SecondCounter = 9;  // һ����һ�εļ�����
  m_i1MinuteCounter = 59;  // һ����һ�εļ�����
  m_i5MinuteCounter = 299;  // һ����һ�εļ�����
  m_i1HourCounter = 3599;  // һ����һ�εļ�����
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
  //����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
  if (GetLocalTime() > m_timeLast) {
    SchedulingTaskPerSecond(GetLocalTime() - m_timeLast);
    m_timeLast = GetLocalTime();
    return true;
  }
  else return false;
}

void CVirtualMarket::ResetMarket(void) {
  ASSERT(0); // ��������û��������г�������
}

void CVirtualMarket::CalculateTime(void) noexcept {
  time(&sm_tLocal);
  m_tMarket = sm_tLocal - m_lTimeZoneOffset;
  gmtime_s(&m_tmMarket, &m_tMarket);
  m_lMarketToday = (m_tmMarket.tm_year + 1900) * 10000 + (m_tmMarket.tm_mon + 1) * 100 + m_tmMarket.tm_mday;
  m_lMarketTime = m_tmMarket.tm_hour * 10000 + m_tmMarket.tm_min * 100 + m_tmMarket.tm_sec;
}

void CVirtualMarket::CalculateLastTradeDay(void) noexcept {
  time_t ttime = 0;

  switch (m_tmMarket.tm_wday) {
  case 1: // ����һ
  ttime = m_tMarket - 3 * 24 * 3600; //
  break;
  case 0: //������
  ttime = m_tMarket - 3 * 24 * 3600; //
  break;
  case 6: // ������
  ttime = m_tMarket - 2 * 24 * 3600; //
  break;
  default: // ����
  ttime = m_tMarket - 24 * 3600; //
  }
  tm tm_;
  gmtime_s(&tm_, &ttime);
  m_lMarketLastTradeDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
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

bool CVirtualMarket::IsWorkingDay(long lDay) noexcept {
  const long year = lDay / 10000;
  const long month = lDay / 100 - year * 100;
  const long day = lDay - year * 10000 - month * 100;
  CTime ct(year, month, day, 12, 0, 0);

  if ((ct.GetDayOfWeek() == 1) || (ct.GetDayOfWeek() == 7)) {
    return false;
  }
  else return true;
}

bool CVirtualMarket::IsEarlyThen(long lEarlyDay, long lLatelyDay, long lTimeSpawnOfDays) noexcept {
  CTimeSpan ts(lTimeSpawnOfDays, 0, 0, 0);
  const long year = lEarlyDay / 10000;
  const long month = lEarlyDay / 100 - year * 100;
  const long day = lEarlyDay - year * 10000 - month * 100;
  CTime ctEarly(year, month, day, 12, 0, 0);
  ctEarly += ts;
  long lNewDay = ctEarly.GetYear() * 10000 + ctEarly.GetMonth() * 100 + ctEarly.GetDay();
  return (lNewDay < lLatelyDay);
}

long CVirtualMarket::GetNextDay(long lDay, long lTimeSpanDays) noexcept {
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

  localtime_s(&tmLocal, &sm_tLocal);
  sprintf_s(buffer, "%02d:%02d:%02d ", tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
  CString str;
  str = buffer;
  return(str);
}

CString CVirtualMarket::GetMarketTimeString(void) {
  char buffer[30];
  tm tmMarket;

  gmtime_s(&tmMarket, &m_tMarket);
  sprintf_s(buffer, "%02d:%02d:%02d ", tmMarket.tm_hour, tmMarket.tm_min, tmMarket.tm_sec);
  CString str;
  str = buffer;
  return(str);
}

void CVirtualMarket::TaskResetMarketFlagAtMidnight(long lCurrentTime) {
  // ��ҹ�������ø��ֱ�ʶ
  if (lCurrentTime <= 1500 && !IsPermitResetMarket()) {  // ����㵽���ʮ��֣�����ϵͳ��ʶ
    m_fPermitResetMarket = true;
    CString str;
    str = m_strMarketId + _T("����ϵͳ���ñ�ʶ");
    TRACE(_T("%S \n"), str.GetBuffer());
    gl_systemMessage.PushInformationMessage(str);
  }
}

bool CVirtualMarket::SchedulingTaskPerSecond(long lSecond) {
  const long lCurrentTime = GetTime();
  //long lCurrentTime2 = GetTime();

  // �����ȳ��򰴼��ʱ���С˳�����У����ʱ�䳤�ı���λ�ڼ��ʱ��̵�֮ǰ��
  SchedulingTaskPerHour(lSecond, lCurrentTime);
  SchedulingTaskPer5Minute(lSecond, lCurrentTime);
  SchedulingTaskPer1Minute(lSecond, lCurrentTime);
  SchedulingTaskPer10Second(lSecond, lCurrentTime);

  return true;
}

bool CVirtualMarket::SchedulingTaskPer10Second(long lSecond, long lCurrentTime) {
  // ����ÿ����һ�ε��������еĶ�ʱ����Ҫ����ʱ�����ӳ��������У�����ִ��ÿ����һ�ε�������ִ��ÿ����һ�ε����������ܹ���֤���������������ִ�С�
  m_i10SecondCounter -= lSecond;
  if (m_i10SecondCounter < 0) {
    m_i10SecondCounter = 9; // ���ü�����
    return true;
  }
  else {
    return false;
  }
}

bool CVirtualMarket::SchedulingTaskPer1Minute(long lSecond, long lCurrentTime) {
  // ����ÿ����һ�ε��������еĶ�ʱ����Ҫ����ʱ�����ӳ��������У�����ִ��ÿ����һ�ε�������ִ��ÿ����һ�ε����������ܹ���֤���������������ִ�С�
  m_i1MinuteCounter -= lSecond;
  if (m_i1MinuteCounter < 0) {
    m_i1MinuteCounter = 59; // ���ü�����
    TaskResetMarketFlagAtMidnight(lCurrentTime);
    return true;
  }
  else {
    return false;
  }
}

bool CVirtualMarket::SchedulingTaskPer5Minute(long lSecond, long lCurrentTime) {
  // ����ÿ����һ�ε��������еĶ�ʱ����Ҫ����ʱ�����ӳ��������У�����ִ��ÿ����һ�ε�������ִ��ÿ����һ�ε����������ܹ���֤���������������ִ�С�
  m_i5MinuteCounter -= lSecond;
  if (m_i5MinuteCounter < 0) {
    m_i5MinuteCounter = 299; // ���ü�����
    return true;
  }
  else {
    return false;
  }
}

bool CVirtualMarket::SchedulingTaskPerHour(long lSecond, long lCurrentTime) {
  // ����ÿ����һ�ε��������еĶ�ʱ����Ҫ����ʱ�����ӳ��������У�����ִ��ÿ����һ�ε�������ִ��ÿ����һ�ε����������ܹ���֤���������������ִ�С�
  m_i1HourCounter -= lSecond;
  if (m_i1HourCounter < 0) {
    m_i1HourCounter = 3599; // ���ü�����
    return true;
  }
  else {
    return false;
  }
}