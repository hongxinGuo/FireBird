#pragma once

#include"stdafx.h"

using namespace std;
#include<atomic>

const int gl_cMaxCalculatingRSThreads = 8;

class CThreadStatus {    // 个线程状态
public:
  CThreadStatus();
  ~CThreadStatus();

  // 计算若干天日线相对强度与否和设置
  void SetCalculatingDayLineRS(bool fFlag) noexcept { m_CalculatingDayLineRelativeStrong = fFlag; }
  bool IsCalculatingDayLineRS(void) noexcept { return m_CalculatingDayLineRelativeStrong; }

  // 实时数据需要计算与否和设置
  void SetRTDataNeedCalculate(bool fFlag) noexcept { m_RTDataNeedCalculate = fFlag; }
  bool IsRTDataNeedCalculate(void) noexcept { return m_RTDataNeedCalculate; }

  // 实时数据计算中与否和设置
  void SetCalculatingRTData(bool fFlag) noexcept { m_CalculatingRTData = fFlag; }
  bool IsCalculatingRTData(void) noexcept { return m_CalculatingRTData; }

  // 临时数据存储中与否和设置
  void SetSavingTempData(bool fFlag) noexcept { m_SavingTempData = fFlag; }
  bool IsSavingTempData(void) noexcept { return m_SavingTempData; }

  // 并发执行计算日线相对强度的计数器，最多允许gl_cMaxCalculatingRSThreads个线程同时执行
  void IncreaseNunberOfCalculatingRSThreads(void) noexcept { m_NumberOfCalculatingRSThreads++; }  // 同时运行线程数加一
  void DecreaseNumberOfCalculatingRSThreads(void) noexcept { m_NumberOfCalculatingRSThreads--; } // 同时运行线程数减一
  bool IsCalculatingRS(void) noexcept { if (m_NumberOfCalculatingRSThreads > 0) return true; else return false; } // 计算日线的线程是否处于运行中
  int HowManyThreadsCalculatingDayLineRS(void) noexcept { return m_NumberOfCalculatingRSThreads; }

  void IncreaseSavingDayLineThreads(void) noexcept { m_SavingDayLine++; }  // 同时运行线程数加一
  void DecreaseSavingDayLineThreads(void) noexcept { m_SavingDayLine--; } // 同时运行线程数减一
  bool IsSavingDayLine(void); // 计算日线的线程是否处于运行中

  void IncreaseRunningThread(void) noexcept { m_NumberOfRunningThread++; }
  void DecreaseRunningThread(void) noexcept { if (m_NumberOfRunningThread > 0) m_NumberOfRunningThread--; }
  int GetNumberOfRunningThread(void) noexcept { int iNumber = m_NumberOfRunningThread; return iNumber; }
  bool IsWorkingThreadRunning(void) noexcept { if (GetNumberOfRunningThread() > 0) return true; else return false; }

protected:
  atomic_bool m_CalculatingDayLineRelativeStrong;
  atomic_bool m_RTDataNeedCalculate;
  atomic_bool m_CalculatingRTData;
  atomic_bool m_SavingTempData;

  atomic_int m_SavingDayLine; // 存储日线历史数据的计数器
  atomic_int m_NumberOfCalculatingRSThreads;  // 正在计算日线相对强度的线程数。目前最多同时允许gl_cMaxCalculatingRSThreads个线程
  atomic_int m_NumberOfRunningThread; // 正在运行的工作线程数量
};
