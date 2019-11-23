#pragma once

#include"stdafx.h"

class CCriticalSectionCounter
{
public:
  CCriticalSectionCounter();
  ~CCriticalSectionCounter();

  // 并发执行计算日线相对强度的计数器，最多允许gl_cMaxCalculatingRSThreads个线程同时执行
  bool IncreasingCounter(void);  // 同时运行线程数加一
  bool DecreasingCounter(void);  // 同时运行线程数减一
  bool IsActive(void);          // 计算日线的线程是否处于运行中

protected:
  int m_iCounter;  // 计数器，最多允许m_MaxNumber个存在
  CCriticalSection m_Lock;
};
