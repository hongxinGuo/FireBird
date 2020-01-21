#include"globedef.h"

#include "VirtualMarket.h"

CVirtualMarket::CVirtualMarket(void) {
  m_fPermitResetSystem = true; // 允许系统被重置标识，唯独此标识不允许系统重置。初始时设置为真：允许重置系统。
  m_fResetSystem = true;
  m_fReadyToRun = true;
}

CVirtualMarket::~CVirtualMarket(void) {
}

bool CVirtualMarket::SchedulingTask(void) {
  static time_t s_timeLast = 0;
  //根据时间，调度各项定时任务.每秒调度一次
  if (gl_systemTime.Gett_time() > s_timeLast) {
    SchedulingTaskPerSecond(gl_systemTime.Gett_time() - s_timeLast);
    s_timeLast = gl_systemTime.Gett_time();
  }
  return true;
}

bool CVirtualMarket::SchedulingTaskPerSecond(long lSecond) {
  const long lCurrentTime = gl_systemTime.GetTime();

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
    ResetSystemFlagAtMidnight(lCurrentTime);
  }
  return true;
}

void CVirtualMarket::ResetSystemFlagAtMidnight(long lCurrentTime) {
  // 午夜过后重置各种标识
  if (lCurrentTime <= 1500 && !IsPermitResetSystem()) {  // 在零点到零点十五分，重置系统标识
    m_fPermitResetSystem = true;
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