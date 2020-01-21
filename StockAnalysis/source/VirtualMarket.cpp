#include"globedef.h"

#include "VirtualMarket.h"

CVirtualMarket::CVirtualMarket(void) {
  m_fPermitResetSystem = true; // ����ϵͳ�����ñ�ʶ��Ψ���˱�ʶ������ϵͳ���á���ʼʱ����Ϊ�棺��������ϵͳ��
  m_fResetSystem = true;
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

bool CVirtualMarket::SchedulingTaskPerSecond(long lSecond) {
  const long lCurrentTime = gl_systemTime.GetTime();

  // �����ȳ��򰴼��ʱ���С˳�����У����ʱ�䳤�ı���λ�ڼ��ʱ��̵�֮ǰ��
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
  static int i1MinuteCounter = 59;  // һ����һ�εļ�����

 // ����ÿ����һ�ε��������еĶ�ʱ����Ҫ����ʱ�����ӳ��������У�����ִ��ÿ����һ�ε�������ִ��ÿ����һ�ε����������ܹ���֤���������������ִ�С�
  if (i1MinuteCounter <= 0) {
    i1MinuteCounter = 59; // ���ü�����
    ResetSystemFlagAtMidnight(lCurrentTime);
  }
  return true;
}

void CVirtualMarket::ResetSystemFlagAtMidnight(long lCurrentTime) {
  // ��ҹ�������ø��ֱ�ʶ
  if (lCurrentTime <= 1500 && !IsPermitResetSystem()) {  // ����㵽���ʮ��֣�����ϵͳ��ʶ
    m_fPermitResetSystem = true;
    CString str;
    str = _T("����ϵͳ���ñ�ʶ");
    gl_systemMessage.PushInformationMessage(str);
  }
}

bool CVirtualMarket::SchedulingTaskPer5Minutes(long lSecond, long lCurrentTime) {
  return true;
}

bool CVirtualMarket::SchedulingTaskPerHour(long lSecond, long lCurrentTime) {
  return true;
}