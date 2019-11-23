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
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
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
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
}

///////////////////////////////////////////////////////////////////////
//
// ����ʶΪ��ʱ�����ñ�ʶΪ�沢�����棻����ʶΪ��ʱ�����ؼ١�
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
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
}