#include "CriticalSectionClock.h"

void CCriticalSectionClock::SetTime(clock_t tt) {
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_tt = tt;
    singleLock.Unlock();
  }
}

clock_t CCriticalSectionClock::GetTime(void) {
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    clock_t tt = m_tt;
    singleLock.Unlock();
    return tt;
  }
  ASSERT(0);
  return 0; // 此分支不可能执行到
}