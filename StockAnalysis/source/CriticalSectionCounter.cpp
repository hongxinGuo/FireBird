#include "CriticalSectionCounter.h"

CCriticalSectionCounter::CCriticalSectionCounter() {
  m_iCounter = 0;
}

CCriticalSectionCounter::~CCriticalSectionCounter() {
}

bool CCriticalSectionCounter::IncreasingCounter(void) {
  bool fSecceed = false;

  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_iCounter++;
    fSecceed = true;
    singleLock.Unlock();
  }
  return fSecceed;
}

bool CCriticalSectionCounter::DecreasingCounter(void) {
  bool fSecceed = false;

  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_iCounter--;
    fSecceed = true;
    singleLock.Unlock();
  }
  return fSecceed;
}

int CCriticalSectionCounter::HowMany(void) {
  return m_iCounter;
}

bool CCriticalSectionCounter::IsActive(void) {
  if (m_iCounter > 0) return true;
  else return false;
}