#include "CriticalSectionCounter.h"

CCriticalSectionCounter::CCriticalSectionCounter()
{
  m_MaxNumber = 1; // Ĭ�ϼ��������Ϊ1������CCriticalSectionBool��Ϊģʽ���ơ�
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
    if (m_iCounter >= m_MaxNumber) { // �������8���߳�ͬʱ���С�������߳����׵���ϵͳ��Ӧ��������û��Ҫ��
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