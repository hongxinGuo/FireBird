#pragma once

#include"afxmt.h"

const int gl_cMaxCalculatingRSThreads = 8;

class CCriticalBool {
public:
  CCriticalBool() { m_fDoing = false; }
  ~CCriticalBool() {}

  void SetFlag(bool fFlag);
  bool IsDoing(void);

protected:
  bool m_fDoing;                // ʵʱ������Ԥ����ã�׼������
  CCriticalSection m_Lock;
};

class CThreadStatus {    // ���߳�״̬
public:
  CThreadStatus();

  // �߳��˳����״̬�����á��˱�־����ϵͳ�˳��������б���رյ��̹߳رպ󷽿�����ϵͳ�˳�������ϵͳ��Ҫ�ȴ���
  void SetExitingThread(bool fFlag) { m_ExitingThread.SetFlag(fFlag); }
  bool IsExitingThread(void) { return m_ExitingThread.IsDoing(); }

  // �����������������ǿ����������
  void SetCalculatingDayLineRS(bool fFlag) { m_CalculateDayLineRelativeStrong.SetFlag(fFlag); }
  bool IsCalculatingDayLineRS(void) { return m_CalculateDayLineRelativeStrong.IsDoing(); }

  // �洢������������
  void SetSavingDayLine(bool fFlag) { m_SavingDayLine.SetFlag(fFlag); }
  bool IsSavingDayLine(void) { return m_SavingDayLine.IsDoing(); }

  // ʵʱ������Ҫ������������
  void SetRTDataNeedCalculate(bool fFlag) { m_RTDataNeedCalculate.SetFlag(fFlag); }
  bool IsRTDataNeedCalculate(void) { return m_RTDataNeedCalculate.IsDoing(); }

  // ʵʱ���ݼ�������������
  void SetCalculatingRTData(bool fFlag) { m_CalculatingRTData.SetFlag(fFlag); }
  bool IsCalculatingRTData(void) { return m_CalculatingRTData.IsDoing(); }

  // ��ʱ���ݴ洢����������
  void SetSavingTempData(bool fFlag) { m_SavingTempData.SetFlag(fFlag); }
  bool IsSavingTempData(void) { return m_SavingTempData.IsDoing(); }

  // ��Ʊ����洢����������
  void SetSavingStockCodeData(bool fFlag) { m_SavingStockCodeData.SetFlag(fFlag); }
  bool IsSavingStockCodeData(void) { return m_SavingStockCodeData.IsDoing(); }

  // ����ִ�м����������ǿ�ȵļ��������������gl_cMaxCalculatingRSThreads���߳�ͬʱִ��
  void IncreaseNunberOfCalculatingRSThreads(void);  // ͬʱ�����߳�����һ
  void DecreaseNumberOfCalculatingRSThreads(void);  // ͬʱ�����߳�����һ
  bool IsCalculatingRSThreadAvailable(void);        // �Ƿ����������µĹ����߳�
  bool IsCalculatingRSThreadRunning(void);          // �������ߵ��߳��Ƿ���������

protected:
  CCriticalBool m_ExitingThread;
  CCriticalBool m_CalculateDayLineRelativeStrong;
  CCriticalBool m_SavingDayLine;
  CCriticalBool m_RTDataNeedCalculate;
  CCriticalBool m_CalculatingRTData;
  CCriticalBool m_SavingTempData;
  CCriticalBool m_SavingStockCodeData;

  int m_iNumberOfCalculatingRSThreads;  // ���ڼ����������ǿ�ȵ��߳�����Ŀǰ���ͬʱ����gl_cMaxCalculatingRSThreads���߳�
  CCriticalSection m_NumberOfCalculatingRSThreads;
};
