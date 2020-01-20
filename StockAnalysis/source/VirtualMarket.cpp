#include"globedef.h"

#include "VirtualMarket.h"

CVirtualMarket::CVirtualMarket(void) {
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

bool CVirtualMarket::SchedulingTaskPerSecond(long lSecondNumber) {
  const long lCurrentTime = gl_systemTime.GetTime();

  // 各调度程序按间隔时间大小顺序排列，间隔时间长的必须位于间隔时间短的之前。
  SchedulingTaskPerHour(lSecondNumber, lCurrentTime);
  SchedulingTaskPer5Minutes(lSecondNumber, lCurrentTime);
  SchedulingTaskPer1Minute(lSecondNumber, lCurrentTime);
  SchedulingTaskPer10Seconds(lSecondNumber, lCurrentTime);

  return true;
}

bool CVirtualMarket::SchedulingTaskPer10Seconds(long lSecondNumber, long lCurrentTime) {
  return true;
}

bool CVirtualMarket::SchedulingTaskPer1Minute(long lSecondNumber, long lCurrentTime) {
  return true;
}

bool CVirtualMarket::SchedulingTaskPer5Minutes(long lSecondNumber, long lCurrentTime) {
  return true;
}

bool CVirtualMarket::SchedulingTaskPerHour(long lSecondNumber, long lCurrentTime) {
  return true;
}