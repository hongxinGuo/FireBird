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
  void SetCalculatingDayLineRS(bool fFlag) noexcept { m_CalculatingDayLineRelativeStrong = fFlag; }
  bool IsCalculatingDayLineRS(void) noexcept { return m_CalculatingDayLineRelativeStrong; }

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
  void IncreaseNunberOfCalculatingRSThreads(void) noexcept { m_NumberOfCalculatingRSThreads++; }  // ͬʱ�����߳�����һ
  void DecreaseNumberOfCalculatingRSThreads(void) noexcept { m_NumberOfCalculatingRSThreads--; } // ͬʱ�����߳�����һ
  bool IsCalculatingRS(void) noexcept { if (m_NumberOfCalculatingRSThreads > 0) return true; else return false; } // �������ߵ��߳��Ƿ���������
  int HowManyThreadsCalculatingDayLineRS(void) noexcept { return m_NumberOfCalculatingRSThreads; }

  void IncreaseSavingDayLineThreads(void) noexcept { m_SavingDayLine++; }  // ͬʱ�����߳�����һ
  void DecreaseSavingDayLineThreads(void) noexcept { m_SavingDayLine--; } // ͬʱ�����߳�����һ
  bool IsSavingDayLine(void); // �������ߵ��߳��Ƿ���������

  void IncreaseRunningThread(void) noexcept { m_NumberOfRunningThread++; }
  void DecreaseRunningThread(void) noexcept { if (m_NumberOfRunningThread > 0) m_NumberOfRunningThread--; }
  int GetNumberOfRunningThread(void) noexcept { int iNumber = m_NumberOfRunningThread; return iNumber; }
  bool IsWorkingThreadRunning(void) noexcept { if (GetNumberOfRunningThread() > 0) return true; else return false; }

protected:
  atomic_bool m_CalculatingDayLineRelativeStrong;
  atomic_bool m_RTDataNeedCalculate;
  atomic_bool m_CalculatingRTData;
  atomic_bool m_SavingTempData;

  atomic_int m_SavingDayLine; // �洢������ʷ���ݵļ�����
  atomic_int m_NumberOfCalculatingRSThreads;  // ���ڼ����������ǿ�ȵ��߳�����Ŀǰ���ͬʱ����gl_cMaxCalculatingRSThreads���߳�
  atomic_int m_NumberOfRunningThread; // �������еĹ����߳�����
};
