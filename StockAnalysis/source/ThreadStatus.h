#pragma once

#include"afxmt.h"

#include"globedef.h"

using namespace std;
#include<atomic>

const int gl_cMaxCalculatingRSThreads = 80;

class CThreadStatus {    // 个线程状态
public:
  CThreadStatus();
  ~CThreadStatus();

  // 线程退出与否状态和设置。此标志用于系统退出，当所有必须关闭的线程关闭后方可允许系统退出，否则系统就要等待。
  void SetExitingThread(bool fFlag) { m_ExitingThread = fFlag; }
  bool IsExitingThread(void) { return m_ExitingThread; }

  // 计算若干天日线相对强度与否和设置
  void SetCalculatingDayLineRS(bool fFlag) { m_CalculateDayLineRelativeStrong = fFlag; }
  bool IsCalculatingDayLineRS(void) { return m_CalculateDayLineRelativeStrong; }

  // 实时数据需要计算与否和设置
  void SetRTDataNeedCalculate(bool fFlag) { m_RTDataNeedCalculate = fFlag; }
  bool IsRTDataNeedCalculate(void) { return m_RTDataNeedCalculate; }

  // 实时数据计算中与否和设置
  void SetCalculatingRTData(bool fFlag) { m_CalculatingRTData = fFlag; }
  bool IsCalculatingRTData(void) { return m_CalculatingRTData; }

  // 临时数据存储中与否和设置
  void SetSavingTempData(bool fFlag) { m_SavingTempData = fFlag; }
  bool IsSavingTempData(void) { return m_SavingTempData; }

  // 股票代码存储中与否和设置
  void SetSavingStockCodeData(bool fFlag) { m_SavingStockCodeData = fFlag; }
  bool IsSavingStockCodeData(void) { return m_SavingStockCodeData; }

  // 并发执行计算日线相对强度的计数器，最多允许gl_cMaxCalculatingRSThreads个线程同时执行
  void IncreaseNunberOfCalculatingRSThreads(void) { m_CounterOfCalculatingRSThreads++; }  // 同时运行线程数加一
  void DecreaseNumberOfCalculatingRSThreads(void) { m_CounterOfCalculatingRSThreads--; } // 同时运行线程数减一
  bool IsCalculatingRS(void) { if (m_CounterOfCalculatingRSThreads > 0) return true; else return false; } // 计算日线的线程是否处于运行中
  int HowManyThreadsCalculatingDayLineRS(void) { return m_CounterOfCalculatingRSThreads; }

  void IncreaseNunberOfSavingDayLineThreads(void) { m_SavingDayLine++; }  // 同时运行线程数加一
  void DecreaseNumberOfSavingDayLineThreads(void) { m_SavingDayLine--; } // 同时运行线程数减一
  bool IsSavingDayLine(void) { if (m_SavingDayLine > 0) return true; else return false; }  // 计算日线的线程是否处于运行中

protected:
  atomic_bool m_ExitingThread;
  atomic_bool m_CalculateDayLineRelativeStrong;
  atomic_bool m_RTDataNeedCalculate;
  atomic_bool m_CalculatingRTData;
  atomic_bool m_SavingTempData;
  atomic_bool m_SavingStockCodeData;

  atomic_int m_SavingDayLine; // 存储日线历史数据的计数器
  atomic_int m_CounterOfCalculatingRSThreads;  // 正在计算日线相对强度的线程数。目前最多同时允许gl_cMaxCalculatingRSThreads个线程
};
