#pragma once

#include"stdafx.h"

class CCriticalSectionCounter
{
public:
  CCriticalSectionCounter();
  ~CCriticalSectionCounter();

  void SetMaxCounter(int iValue) noexcept { m_MaxNumber = iValue; }
  // 并发执行计算日线相对强度的计数器，最多允许gl_cMaxCalculatingRSThreads个线程同时执行
  void IncreaseCounter(void);  // 同时运行线程数加一
  void DecreaseCounter(void);  // 同时运行线程数减一
  bool IsCounterAvailable(void);        // 是否允许生成新的工作线程
  bool IsActive(void);          // 计算日线的线程是否处于运行中

protected:
  int m_iCounter;  // 计数器，最多允许m_MaxNumber个存在
  CCriticalSection m_Lock;
  int m_MaxNumber;
};
