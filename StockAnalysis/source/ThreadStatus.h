#pragma once

#include"stdafx.h"

using namespace std;
#include<atomic>

const int gl_cMaxCalculatingRSThreads = 8;

class CThreadStatus {    // ���߳�״̬
public:
  CThreadStatus();
  ~CThreadStatus();

  // �����������������ǿ����������
  void SetCalculatingDayLineRS(bool fFlag) noexcept { m_CalculateDayLineRelativeStrong = fFlag; }
  bool IsCalculatingDayLineRS(void) noexcept { return m_CalculateDayLineRelativeStrong; }

  // ʵʱ������Ҫ������������
  void SetRTDataNeedCalculate(bool fFlag) noexcept { m_RTDataNeedCalculate = fFlag; }
  bool IsRTDataNeedCalculate(void) noexcept { return m_RTDataNeedCalculate; }

  // ʵʱ���ݼ�������������
  void SetCalculatingRTData(bool fFlag) noexcept { m_CalculatingRTData = fFlag; }
  bool IsCalculatingRTData(void) noexcept { return m_CalculatingRTData; }

  // ��ʱ���ݴ洢����������
  void SetSavingTempData(bool fFlag) noexcept { m_SavingTempData = fFlag; }
  bool IsSavingTempData(void) noexcept { return m_SavingTempData; }

  // ����ִ�м����������ǿ�ȵļ��������������gl_cMaxCalculatingRSThreads���߳�ͬʱִ��
  void IncreaseNunberOfCalculatingRSThreads(void) noexcept { m_CounterOfCalculatingRSThreads++; }  // ͬʱ�����߳�����һ
  void DecreaseNumberOfCalculatingRSThreads(void) noexcept { m_CounterOfCalculatingRSThreads--; } // ͬʱ�����߳�����һ
  bool IsCalculatingRS(void) noexcept { if (m_CounterOfCalculatingRSThreads > 0) return true; else return false; } // �������ߵ��߳��Ƿ���������
  int HowManyThreadsCalculatingDayLineRS(void) noexcept { return m_CounterOfCalculatingRSThreads; }

  void IncreaseNunberOfSavingDayLineThreads(void) noexcept { m_SavingDayLine++; }  // ͬʱ�����߳�����һ
  void DecreaseNumberOfSavingDayLineThreads(void) noexcept { m_SavingDayLine--; } // ͬʱ�����߳�����һ
  bool IsSavingDayLine(void); // �������ߵ��߳��Ƿ���������

  void IncreaseNumberOfRunningThread(void) noexcept { m_NumberOfRunningThread++; }
  void DecreaseNumberOfRunningThread(void) noexcept { if (m_NumberOfRunningThread > 0) m_NumberOfRunningThread--; }
  int GetNumberOfRunningThread(void) noexcept { int iNumber = m_NumberOfRunningThread; return iNumber; }
  bool IsWorkingThreadRunning(void) noexcept { if (GetNumberOfRunningThread() > 0) return true; else return false; }

protected:
  atomic_bool m_CalculateDayLineRelativeStrong;
  atomic_bool m_RTDataNeedCalculate;
  atomic_bool m_CalculatingRTData;
  atomic_bool m_SavingTempData;

  atomic_int m_SavingDayLine; // �洢������ʷ���ݵļ�����
  atomic_int m_CounterOfCalculatingRSThreads;  // ���ڼ����������ǿ�ȵ��߳�����Ŀǰ���ͬʱ����gl_cMaxCalculatingRSThreads���߳�
  atomic_int m_NumberOfRunningThread; // �������еĹ����߳�����
};
