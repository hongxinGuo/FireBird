#pragma once

#include"afxmt.h"

#include"CriticalSectionBool.h"

const int gl_cMaxCalculatingRSThreads = 8;

class CThreadStatus {    // 个线程状态
public:
  CThreadStatus();

  // 线程退出与否状态和设置。此标志用于系统退出，当所有必须关闭的线程关闭后方可允许系统退出，否则系统就要等待。
  void SetExitingThread(bool fFlag) { m_ExitingThread.SetFlag(fFlag); }
  bool IsExitingThread(void) { return m_ExitingThread.IsTrue(); }

  // 计算若干天日线相对强度与否和设置
  void SetCalculatingDayLineRS(bool fFlag) { m_CalculateDayLineRelativeStrong.SetFlag(fFlag); }
  bool IsCalculatingDayLineRS(void) { return m_CalculateDayLineRelativeStrong.IsTrue(); }

  // 存储日线与否和设置
  void SetSavingDayLine(bool fFlag) { m_SavingDayLine.SetFlag(fFlag); }
  bool IsSavingDayLine(void) { return m_SavingDayLine.IsTrue(); }

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
  void IncreaseNunberOfCalculatingRSThreads(void);  // 同时运行线程数加一
  void DecreaseNumberOfCalculatingRSThreads(void);  // 同时运行线程数减一
  bool IsCalculatingRSThreadAvailable(void);        // 是否允许生成新的工作线程
  bool IsCalculatingRSThreadRunning(void);          // 计算日线的线程是否处于运行中

protected:
  CCriticalSectionBool m_ExitingThread;
  CCriticalSectionBool m_CalculateDayLineRelativeStrong;
  CCriticalSectionBool m_SavingDayLine;
  CCriticalSectionBool m_RTDataNeedCalculate;
  CCriticalSectionBool m_CalculatingRTData;
  CCriticalSectionBool m_SavingTempData;
  CCriticalSectionBool m_SavingStockCodeData;

  int m_iNumberOfCalculatingRSThreads;  // 正在计算日线相对强度的线程数。目前最多同时允许gl_cMaxCalculatingRSThreads个线程
  CCriticalSection m_NumberOfCalculatingRSThreads;
};
