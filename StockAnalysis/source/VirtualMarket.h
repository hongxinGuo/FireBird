#pragma once

#include"stdafx.h"

class CVirtualMarket : public CObject {
public:
  CVirtualMarket(void);
  ~CVirtualMarket(void);

  virtual bool SchedulingTask(void); // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
  virtual void ResetMarket(void) = 0;

  bool SchedulingTaskPerSecond(long lSecondNumber); // ÿ�����һ��
  bool SchedulingTaskPer10Seconds(long lSecondNumber, long lCurrentTime); // ÿʮ�����һ��
  bool SchedulingTaskPer1Minute(long lSecondNumber, long lCurrentTime); // ÿһ���ӵ���һ��
  void ResetSystemFlagAtMidnight(long lCurrentTime);
  bool SchedulingTaskPer5Minutes(long lSecondNumber, long lCurrentTime); // ÿ����ӵ���һ��
  bool SchedulingTaskPerHour(long lSecondNumber, long lCurrentTime); // ÿСʱ����һ��

  bool IsReadyToRun(void) noexcept { return m_fReadyToRun; }
  void SetReadyToRun(bool fFlag) noexcept { m_fReadyToRun = fFlag; }
  bool IsPermitResetSystem(void) noexcept { return m_fPermitResetSystem; }
  void SetPermitResetSystem(bool fFlag) noexcept { m_fPermitResetSystem = fFlag; }
  bool IsResetSystem(void) noexcept { return m_fResetSystem; }
  void SetResetSystem(bool fFlag) noexcept { m_fResetSystem = fFlag; }

protected:
  bool m_fPermitResetSystem; // ��������ϵͳ��������ϻ��������еĻ�����Ҫÿ������ϵͳ
  bool m_fResetSystem; // ����ϵͳ��ʶ
  bool m_fReadyToRun;
};
