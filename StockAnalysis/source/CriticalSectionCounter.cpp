#include "CriticalSectionCounter.h"

CCriticalSectionCounter::CCriticalSectionCounter()
{
  m_MaxNumber = 1; // 默认计数器最大为1，即与CCriticalSectionBool行为模式相似。
}

CCriticalSectionCounter::~CCriticalSectionCounter() {
}

void CCriticalSectionCounter::IncreaseCounter(void)
{
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    ASSERT(m_iCounter < m_MaxNumber);
    m_iCounter++;
    singleLock.Unlock();
  }
}

void CCriticalSectionCounter::DecreaseCounter(void)
{
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    ASSERT(m_iCounter > 0);
    m_iCounter--;
    singleLock.Unlock();
  }
}

bool CCriticalSectionCounter::IsCounterAvailable(void)
{
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    bool fFlag;
    if (m_iCounter >= m_MaxNumber) { // 最多允许8个线程同时运行。更多的线程容易导致系统响应变慢，且没必要。
      fFlag = false;
    }
    else fFlag = true;
    singleLock.Unlock();
    return fFlag;
  }
}

bool CCriticalSectionCounter::IsActive(void)
{
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    bool fFlag;
    if (m_iCounter > 0) {
      fFlag = true;
    }
    else fFlag = false;
    singleLock.Unlock();

    return fFlag;
  }
}