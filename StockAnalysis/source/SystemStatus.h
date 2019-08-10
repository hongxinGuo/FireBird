#pragma once

#include"afxmt.h"

class CSystemStatus {    // 日线数据结构
public:
  CSystemStatus();

  void SetExitingClientThreadInProcess(bool fFlag);
  bool IsExitingClientThreadInProcess(void);

  void SetCalculateRSInProcess(bool fFlag);
  bool IsCalculateRSInProcess(void);

  void SetDayLineReadingInProcess(bool fFlag);
  bool IsDayLineReadingInProcess(void);

  void SetSavingDayLineInProcess(bool fFlag);
  bool IsSavingDayLineInProcess(void);

  void SetRTDataReadingInProcess(bool fFlag);
  bool IsRTDataReadingInProcess(void);

  void SetRTDataReceived(bool fFlag);
  bool IsRTDataReceived(void);
  
  void SetRTDataNeedCalculate(bool fFlag);
  bool IsRTDataNeedCalculate(void);

  void SetDayLineDataReady(bool fFlag);
  bool IsDayLineDataReady(void);

  void SetCalculatingRTData(bool fFlag);
  bool IsCalculatingRTData(void);
  
  void SetSavingTempData(bool fFlag);
  bool IsSavingTempData(void);

  void SetSavingStockCodeData(bool fFlag);
  bool IsSavingStockCodeData(void);


protected:
  bool m_fExitingClientThreadInProcess;                // 要求各工作线程退出
  CCriticalSection m_ExitingClientThreadInProcessLock;   

  bool m_fCalculatingRelativeStrongInProcess; // 是否处于计算相对强度的过程中标识
  CCriticalSection m_CalculateRSInProcessLock;

  bool m_fDayLineReadingInProcess; // 是否处于提取日线历史数据中标识
  CCriticalSection m_DayLineReadingInProcessLock;

  bool m_fSavingDayLineInProcess; // 是否处于存储日线历史数据的数据库操作中。
  CCriticalSection m_SavingDayLineInProcessLock;

  bool m_fRTDataReadingInProcess;  // 接收实时数据线程是否执行标识
  CCriticalSection m_RTDataReadingInProcessLock;

  bool m_fRTDataReceived;             // 实时数据已接收完成标识
  CCriticalSection m_RTDataReceivedLock;

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

};
