#pragma once

#include"stdafx.h"

class CCriticalSectionClock {
public:
  CCriticalSectionClock() { m_tt = 0; }
  ~CCriticalSectionClock() {}

  void SetTime(clock_t tt);
  clock_t GetTime(void);

protected:
  clock_t m_tt;
  CCriticalSection m_Lock;
};