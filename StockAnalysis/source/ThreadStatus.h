#pragma once

#include"stdafx.h"

using namespace std;
#include<atomic>

constexpr int cMaxBackGroundTaskThreads = 8;

class CThreadStatus {    // ���߳�״̬
public:
  CThreadStatus();
  ~CThreadStatus();

  // �����������������ǿ����������
  void SetCalculatingDayLineRS(bool fFlag) noexcept { m_CalculatingDayLineRS = fFlag; }
  bool IsCalculatingDayLineRS(void) noexcept { return m_CalculatingDayLineRS; }

  // �����������������ǿ����������
  void SetCalculatingWeekLineRS(bool fFlag) noexcept { m_CalculatingWeekLineRS = fFlag; }
  bool IsCalculatingWeekLineRS(void) noexcept { return m_CalculatingWeekLineRS; }

  // ʵʱ������Ҫ������������
  void SetRTDataNeedCalculate(bool fFlag) noexcept { m_RTDataNeedCalculate = fFlag; }
  bool IsRTDataNeedCalculate(void) noexcept { return m_RTDataNeedCalculate; }

  // ʵʱ���ݼ�������������
  void SetCalculatingRTData(bool fFlag) noexcept { m_CalculatingRTData = fFlag; }
  bool IsCalculatingRTData(void) noexcept { return m_CalculatingRTData; }

  // ��ʱ���ݴ洢����������
  void SetSavingTempData(bool fFlag) noexcept { m_SavingTempData = fFlag; }
  bool IsSavingTempData(void) noexcept { return m_SavingTempData; }

  // ����������ʷ����
  void SetCreatingWeekLine(bool fFlag) noexcept { m_CreatingWeekLine = fFlag; }
  bool IsCreatingWeekLine(void) noexcept { return m_CreatingWeekLine; }

  // ����ִ�м����������ǿ�ȵļ��������������cMaxBackGroundTaskThreads���߳�ͬʱִ��
  void IncreaseBackGroundWorkingthreads(void) noexcept { m_NumberOfBackGroundWorkingThreads++; }  // ͬʱ�����߳�����һ
  void DecreaseBackGroundWorkingthreads(void) noexcept { m_NumberOfBackGroundWorkingThreads--; } // ͬʱ�����߳�����һ
  bool IsBackGroundthreadsWorking(void) noexcept { if (m_NumberOfBackGroundWorkingThreads > 0) return true; else return false; } // �������ߵ��߳��Ƿ���������
  int HowManyBackGroundThreadsWorking(void) noexcept { return m_NumberOfBackGroundWorkingThreads; }

  void IncreaseSavingDayLineThreads(void) noexcept { m_SavingDayLine++; }  // ͬʱ�����߳�����һ
  void DecreaseSavingDayLineThreads(void) noexcept { m_SavingDayLine--; } // ͬʱ�����߳�����һ
  bool IsSavingDayLine(void); // �������ߵ��߳��Ƿ���������

  void IncreaseRunningThread(void) noexcept { m_NumberOfRunningThread++; }
  void DecreaseRunningThread(void) noexcept { if (m_NumberOfRunningThread > 0) m_NumberOfRunningThread--; }
  int GetNumberOfRunningThread(void) noexcept { const int iNumber = m_NumberOfRunningThread; return iNumber; }
  bool IsWorkingThreadRunning(void) noexcept { if (GetNumberOfRunningThread() > 0) return true; else return false; }

protected:
  atomic_bool m_CalculatingDayLineRS;
  atomic_bool m_CalculatingWeekLineRS;
  atomic_bool m_RTDataNeedCalculate;
  atomic_bool m_CalculatingRTData;
  atomic_bool m_SavingTempData;
  atomic_bool m_CreatingWeekLine;

  atomic_int m_SavingDayLine; // �洢������ʷ���ݵļ�����
  atomic_int m_NumberOfBackGroundWorkingThreads;  // ���ڼ����������ǿ�ȵ��߳�����Ŀǰ���ͬʱ����cMaxBackGroundTaskThreads���߳�
  atomic_int m_NumberOfRunningThread; // �������еĹ����߳�����
};
