#include"globedef.h"

#include"WebDataInquirer.h"
#include "PotenDailyBriefMarket.h"

bool CPotenDailyBriefingMarket::SchedulingTask(void) {
  static time_t s_timeLast = 0;

  //����ʱ�䣬���ȸ��ʱ����.ÿ�����һ��
  if (gl_systemTime.Gett_time() > s_timeLast) {
    SchedulingTaskPerSecond(gl_systemTime.Gett_time() - s_timeLast);
    s_timeLast = gl_systemTime.Gett_time();
  }
  return false;
}

bool CPotenDailyBriefingMarket::SchedulingTaskPerSecond(long lSecond) {
  gl_WebDataInquirer.GetPotenDailyBriefingData();

  return true;
}