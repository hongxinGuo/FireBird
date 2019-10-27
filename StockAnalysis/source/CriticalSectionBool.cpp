#include "CriticalSectionBool.h"

void CCriticalSectionBool::SetFlag(bool fFlag) {
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fFlag = fFlag;
    singleLock.Unlock();
  }
}

bool CCriticalSectionBool::IsTrue(void) {
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fFlag;
    singleLock.Unlock();
    return fFlag;
  }
  ASSERT(0);
  return false; // 此分支不可能执行到，只为了消除编译器的警告而存在
}

bool CCriticalSectionBool::CheckTrueAndThenClearIt(void)
{
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fFlag;
    if (fFlag) {
      m_fFlag = false;
    }
    singleLock.Unlock();
    return fFlag;
  }
  ASSERT(0);
  return false; // 此分支不可能执行到，只为了消除编译器的警告而存在
}

///////////////////////////////////////////////////////////////////////
//
// 当标识为假时，设置标识为真并返回真；当标识为真时，返回假。
//
////////////////////////////////////////////////////////////////////////
bool CCriticalSectionBool::CheckFalseAndThenSetIt(void)
{
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fFlag;
    if (!fFlag) {
      m_fFlag = true;
      singleLock.Unlock();
      return true;
    }
    else {
      singleLock.Unlock();
      return false;
    }
  }
  ASSERT(0);
  return false; // 此分支不可能执行到，只为了消除编译器的警告而存在
}