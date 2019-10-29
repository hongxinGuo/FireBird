#pragma once

#include"afxmt.h"

#include"globedef.h"

#include"CriticalSectionBool.h"
#include"CriticalSectionCounter.h"

const int gl_cMaxCalculatingRSThreads = 8;

class CThreadStatus {    // ���߳�״̬
public:
  CThreadStatus();
  ~CThreadStatus();

  // �߳��˳����״̬�����á��˱�־����ϵͳ�˳��������б���رյ��̹߳رպ󷽿�����ϵͳ�˳�������ϵͳ��Ҫ�ȴ���
  void SetExitingThread(bool fFlag) { m_ExitingThread.SetFlag(fFlag); }
  bool IsExitingThread(void) { return m_ExitingThread.IsTrue(); }

  // �����������������ǿ����������
  void SetCalculatingDayLineRS(bool fFlag) { m_CalculateDayLineRelativeStrong.SetFlag(fFlag); }
  bool IsCalculatingDayLineRS(void) { return m_CalculateDayLineRelativeStrong.IsTrue(); }

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
  void IncreaseNunberOfCalculatingRSThreads(void) { m_CounterOfCalculatingRSThreads.IncreaseCounter(); }  // ͬʱ�����߳�����һ
  void DecreaseNumberOfCalculatingRSThreads(void) { m_CounterOfCalculatingRSThreads.DecreaseCounter(); } // ͬʱ�����߳�����һ
  bool IsCalculatingRSThreadAvailable(void) { return m_CounterOfCalculatingRSThreads.IsCounterAvailable(); }  // �Ƿ����������µĹ����߳�
  bool IsCalculatingRS(void) { return m_CounterOfCalculatingRSThreads.IsActive(); }  // �������ߵ��߳��Ƿ���������

  void IncreaseNunberOfSavingDayLineThreads(void) { m_SavingDayLine.IncreaseCounter(); }  // ͬʱ�����߳�����һ
  void DecreaseNumberOfSavingDayLineThreads(void) { m_SavingDayLine.DecreaseCounter(); } // ͬʱ�����߳�����һ
  bool IsSavingDayLineThreadAvailable(void) { return m_SavingDayLine.IsCounterAvailable(); }  // �Ƿ����������µĹ����߳�
  bool IsSavingDayLine(void) { return m_SavingDayLine.IsActive(); }  // �������ߵ��߳��Ƿ���������

protected:
  CCriticalSectionBool m_ExitingThread;
  CCriticalSectionBool m_CalculateDayLineRelativeStrong;
  CCriticalSectionBool m_RTDataNeedCalculate;
  CCriticalSectionBool m_CalculatingRTData;
  CCriticalSectionBool m_SavingTempData;
  CCriticalSectionBool m_SavingStockCodeData;

  CCriticalSectionCounter m_SavingDayLine; // ���Ҫ��Ϊ���ͣ��������8������RSthreads���ơ�
  CCriticalSectionCounter m_CounterOfCalculatingRSThreads;  // ���ڼ����������ǿ�ȵ��߳�����Ŀǰ���ͬʱ����gl_cMaxCalculatingRSThreads���߳�
};
