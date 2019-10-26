#include "CriticalSectionBool.h"

void CCriticalSectionBool::SetFlag(bool fFlag) {
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fDoing = fFlag;
    singleLock.Unlock();
  }
}

bool CCriticalSectionBool::IsTrue(void) {
  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fDoing;
    singleLock.Unlock();
    return fFlag;
  }
  ASSERT(0);
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
}