#include"globedef.h"

#include "VirtualMarket.h"

CVirtualMarket::CVirtualMarket(void) {
  m_fReadyToRun = true;
}

CVirtualMarket::~CVirtualMarket(void) {
}

bool CVirtualMarket::SchedulingTask(void) {
  static time_t s_timeLast = 0;
  //����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
  if (gl_systemTime.Gett_time() > s_timeLast) {
    SchedulingTaskPerSecond(gl_systemTime.Gett_time() - s_timeLast);
    s_timeLast = gl_systemTime.Gett_time();
  }
  return true;
}

bool CVirtualMarket::SchedulingTaskPerSecond(long lSecondNumber) {
  const long lCurrentTime = gl_systemTime.GetTime();

  // �����ȳ��򰴼��ʱ���С˳�����У����ʱ�䳤�ı���λ�ڼ��ʱ��̵�֮ǰ��
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