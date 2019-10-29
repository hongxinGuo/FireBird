#pragma once

#include"afxmt.h"

#include"globedef.h"

#include"CriticalSectionBool.h"
#include"CriticalSectionCounter.h"

const int gl_cMaxCalculatingRSThreads = 8;

class CThreadStatus {    // 个线程状态
public:
  CThreadStatus();
  ~CThreadStatus();

  // 线程退出与否状态和设置。此标志用于系统退出，当所有必须关闭的线程关闭后方可允许系统退出，否则系统就要等待。
  void SetExitingThread(bool fFlag) { m_ExitingThread.SetFlag(fFlag); }
  bool IsExitingThread(void) { return m_ExitingThread.IsTrue(); }

  // 计算若干天日线相对强度与否和设置
  void SetCalculatingDayLineRS(bool fFlag) { m_CalculateDayLineRelativeStrong.SetFlag(fFlag); }
  bool IsCalculatingDayLineRS(void) { return m_CalculateDayLineRelativeStrong.IsTrue(); }

  // 实时数据需要计算与否和设置
  void SetRTDataNeedCalculate(bool fFlag) { m_RTDataNeedCalculate.SetFlag(fFlag); }
  bool IsRTDataNeedCalculate(void) { return m_RTDataNeedCalculate.IsTrue(); }

  // 实时数据计算中与否和设置
  void SetCalculatingRTData(bool fFlag) { m_CalculatingRTData.SetFlag(fFlag); }
  bool IsCalculatingRTData(void) { return m_CalculatingRTData.IsTrue(); }

  // 临时数据存储中与否和设置
  void SetSavingTempData(bool fFlag) { m_SavingTempData.SetFlag(fFlag); }
  bool IsSavingTempData(void) { return m_SavingTempData.IsTrue(); }

  // 股票代码存储中与否和设置
  void SetSavingStockCodeData(bool fFlag) { m_SavingStockCodeData.SetFlag(fFlag); }
  bool IsSavingStockCodeData(void) { return m_SavingStockCodeData.IsTrue(); }

  // 并发执行计算日线相对强度的计数器，最多允许gl_cMaxCalculatingRSThreads个线程同时执行
  void IncreaseNunberOfCalculatingRSThreads(void) { m_CounterOfCalculatingRSThreads.IncreaseCounter(); }  // 同时运行线程数加一
  void DecreaseNumberOfCalculatingRSThreads(void) { m_CounterOfCalculatingRSThreads.DecreaseCounter(); } // 同时运行线程数减一
  bool IsCalculatingRSThreadAvailable(void) { return m_CounterOfCalculatingRSThreads.IsCounterAvailable(); }  // 是否允许生成新的工作线程
  bool IsCalculatingRS(void) { return m_CounterOfCalculatingRSThreads.IsActive(); }  // 计算日线的线程是否处于运行中

  void IncreaseNunberOfSavingDayLineThreads(void) { m_SavingDayLine.IncreaseCounter(); }  // 同时运行线程数加一
  void DecreaseNumberOfSavingDayLineThreads(void) { m_SavingDayLine.DecreaseCounter(); } // 同时运行线程数减一
  bool IsSavingDayLineThreadAvailable(void) { return m_SavingDayLine.IsCounterAvailable(); }  // 是否允许生成新的工作线程
  bool IsSavingDayLine(void) { return m_SavingDayLine.IsActive(); }  // 计算日线的线程是否处于运行中

protected:
  CCriticalSectionBool m_ExitingThread;
  CCriticalSectionBool m_CalculateDayLineRelativeStrong;
  CCriticalSectionBool m_RTDataNeedCalculate;
  CCriticalSectionBool m_CalculatingRTData;
  CCriticalSectionBool m_SavingTempData;
  CCriticalSectionBool m_SavingStockCodeData;

  CCriticalSectionCounter m_SavingDayLine; // 这个要改为整型，最多允许8个，与RSthreads相似。
  CCriticalSectionCounter m_CounterOfCalculatingRSThreads;  // 正在计算日线相对强度的线程数。目前最多同时允许gl_cMaxCalculatingRSThreads个线程
};
