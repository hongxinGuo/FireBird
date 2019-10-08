#pragma once

#include"afxmt.h"

class CThreadStatus {    // 个线程状态
public:
  CThreadStatus();

  // 线程退出与否状态和设置。此标志用于系统退出，当所有必须关闭的线程关闭后方可允许系统退出，否则系统就要等待。
  void SetExitingClientThreadInProcess(bool fFlag);
  bool IsExitingClientThreadInProcess(void);

  // 计算若干天日线相对强度与否和设置
  void SetCalculateDayLineRS(bool fFlag);
  bool IsCalculateDayLineRS(void);

  // 读取日线与否和设置
  void SetNeteaseDayLineReadingInProcess(bool fFlag);
  bool IsNeteaseDayLineReadingInProcess(void);

  // 存储日线与否和设置
  void SetSavingDayLineInProcess(bool fFlag);
  bool IsSavingDayLineInProcess(void);

  // 接收新浪实时数据与否和设置
  void SetSinaRTDataReadingInProcess(bool fFlag);
  bool IsSinaRTDataReadingInProcess(void);

  // 接收腾讯实时数据与否和设置
  void SetTengxunRTDataReadingInProcess(bool fFlag);
  bool IsTengxunRTDataReadingInProcess(void);

  // 新浪实时数据接收到与否和设置
  void SetSinaRTDataReceived(bool fFlag);
  bool IsSinaRTDataReceived(void);

  // 腾讯实时数据接受到与否和设置
  void SetTengxunRTDataReceived(bool fFlag);
  bool IsTengxunRTDataReceived(void);

  // 实时数据需要计算与否和设置
  void SetRTDataNeedCalculate(bool fFlag);
  bool IsRTDataNeedCalculate(void);

  // 日线数据准备好与否和设置
  void SetDayLineDataReady(bool fFlag);
  bool IsDayLineDataReady(void);

  // 实时数据计算中与否和设置
  void SetCalculatingRTData(bool fFlag);
  bool IsCalculatingRTData(void);

  // 临时数据存储中与否和设置
  void SetSavingTempData(bool fFlag);
  bool IsSavingTempData(void);

  // 股票代码存储中与否和设置
  void SetSavingStockCodeData(bool fFlag);
  bool IsSavingStockCodeData(void);

  // 并发执行计算日线相对强度的计数器，最多允许16个线程同时执行
  void IncreaseNunberOfCalculatingRSThreads(void);  // 同时运行线程数加一
  void DecreaseNumberOfCalculatingRSThreads(void);  // 同时运行线程数减一
  bool IsCalculatingRSThreadAvailable(void);        // 是否允许生成新的工作线程
  bool IsCalculatingRSThreadRunning(void);          // 计算日线的线程是否处于运行中

protected:
  bool m_fExitingClientThreadInProcess;                // 要求各工作线程退出
  CCriticalSection m_ExitingClientThreadInProcessLock;

  bool m_fCalculatingDayLineRelativeStrong; // 是否处于计算相对强度的过程中标识
  CCriticalSection m_CalculateDayLineRSLock;

  bool m_fNeteaseDayLineReadingInProcess; // 是否处于提取日线历史数据中标识
  CCriticalSection m_NeteaseDayLineReadingInProcessLock;

  bool m_fSavingDayLineInProcess; // 是否处于存储日线历史数据的数据库操作中。
  CCriticalSection m_SavingDayLineInProcessLock;

  bool m_fSinaRTDataReadingInProcess;  // 接收新浪实时数据线程是否执行标识
  CCriticalSection m_SinaRTDataReadingInProcessLock;

  bool m_fTengxunRTDataReadingInProcess;  // 接收腾讯实时数据线程是否执行标识
  CCriticalSection m_TengxunRTDataReadingInProcessLock;

  bool m_fSinaRTDataReceived;             // 新浪实时数据已接收完成标识
  CCriticalSection m_SinaRTDataReceivedLock;

  bool m_fTengxunRTDataReceived;             // 腾讯实时数据已接收完成标识
  CCriticalSection m_TengxunRTDataReceivedLock;

  bool m_fRTDataNeedCalculate;                // 实时数据已预处理好，准备计算
  CCriticalSection m_RTDataNeedCalculateLock;

  bool m_fDayLineDataReady;      // 日线数据已提取完成标识
  CCriticalSection m_DayLineDataReadyLock;

  bool m_fCalculatingRTData; // 实时数据计算线程工作状态标识
  CCriticalSection m_CalculatingRTDataLock;

  bool m_fSavingTempData; // 计算出的临时数据存储状态标识
  CCriticalSection m_SavingTempDataLock;

  bool m_fSavingStockCodeData; // 股票代码数据存储状态标识
  CCriticalSection m_SavingStockCodeDataLock;

  int m_iNumberOfCalculatingRSThreads;  // 正在计算日线相对强度的线程数。目前最多同时允许16个线程
  CCriticalSection m_NumberOfCalculatingRSThreads;
};
