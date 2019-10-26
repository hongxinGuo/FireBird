#pragma once

#include"afxmt.h"

#include"CriticalSectionBool.h"

const int gl_cMaxCalculatingRSThreads = 8;

class CThreadStatus {    // ���߳�״̬
public:
  CThreadStatus();

  // �߳��˳����״̬�����á��˱�־����ϵͳ�˳��������б���رյ��̹߳رպ󷽿�����ϵͳ�˳�������ϵͳ��Ҫ�ȴ���
  void SetExitingThread(bool fFlag) { m_ExitingThread.SetFlag(fFlag); }
  bool IsExitingThread(void) { return m_ExitingThread.IsTrue(); }

  // �����������������ǿ����������
  void SetCalculatingDayLineRS(bool fFlag) { m_CalculateDayLineRelativeStrong.SetFlag(fFlag); }
  bool IsCalculatingDayLineRS(void) { return m_CalculateDayLineRelativeStrong.IsTrue(); }

  // �洢������������
  void SetSavingDayLine(bool fFlag) { m_SavingDayLine.SetFlag(fFlag); }
  bool IsSavingDayLine(void) { return m_SavingDayLine.IsTrue(); }

  // ʵʱ������Ҫ������������
  void SetRTDataNeedCalculate(bool fFlag) { m_RTDataNeedCalculate.SetFlag(fFlag); }
  bool IsRTDataNeedCalculate(void) { return m_RTDataNeedCalculate.IsTrue(); }

  // ʵʱ���ݼ�������������
  void SetCalculatingRTData(bool fFlag) { m_CalculatingRTData.SetFlag(fFlag); }
  bool IsCalculatingRTData(void) { return m_CalculatingRTData.IsTrue(); }

  // ��ʱ���ݴ洢����������
  void SetSavingTempData(bool fFlag) { m_SavingTempData.SetFlag(fFlag); }
  bool IsSavingTempData(void) { return m_SavingTempData.IsTrue(); }

  // ��Ʊ����洢����������
  void SetSavingStockCodeData(bool fFlag) { m_SavingStockCodeData.SetFlag(fFlag); }
  bool IsSavingStockCodeData(void) { return m_SavingStockCodeData.IsTrue(); }

  // ����ִ�м����������ǿ�ȵļ��������������gl_cMaxCalculatingRSThreads���߳�ͬʱִ��
  void IncreaseNunberOfCalculatingRSThreads(void);  // ͬʱ�����߳�����һ
  void DecreaseNumberOfCalculatingRSThreads(void);  // ͬʱ�����߳�����һ
  bool IsCalculatingRSThreadAvailable(void);        // �Ƿ����������µĹ����߳�
  bool IsCalculatingRSThreadRunning(void);          // �������ߵ��߳��Ƿ���������

protected:
  CCriticalSectionBool m_ExitingThread;
  CCriticalSectionBool m_CalculateDayLineRelativeStrong;
  CCriticalSectionBool m_SavingDayLine;
  CCriticalSectionBool m_RTDataNeedCalculate;
  CCriticalSectionBool m_CalculatingRTData;
  CCriticalSectionBool m_SavingTempData;
  CCriticalSectionBool m_SavingStockCodeData;

  int m_iNumberOfCalculatingRSThreads;  // ���ڼ����������ǿ�ȵ��߳�����Ŀǰ���ͬʱ����gl_cMaxCalculatingRSThreads���߳�
  CCriticalSection m_NumberOfCalculatingRSThreads;
};
