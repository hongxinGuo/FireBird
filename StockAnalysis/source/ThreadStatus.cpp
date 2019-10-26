#include"stdafx.h"

#include"globedef.h"

#include"ThreadStatus.h"
#include"SystemMessage.h"

CThreadStatus::CThreadStatus() {
  static int siInstance = 0;
  if (siInstance++ > 0) {
    TRACE("ϵͳ״ֻ̬��������һ��ʵ��\n");
    gl_systemMessage.PushInformationMessage(_T("ϵͳ״ֻ̬��������һ��ʵ��"));
  }

  m_iNumberOfCalculatingRSThreads = 0;
}

void CThreadStatus::IncreaseNunberOfCalculatingRSThreads(void) {
  CSingleLock singleLock(&m_NumberOfCalculatingRSThreads);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    ASSERT(m_iNumberOfCalculatingRSThreads < gl_cMaxCalculatingRSThreads);
    m_iNumberOfCalculatingRSThreads++;
    singleLock.Unlock();
  }
}

void CThreadStatus::DecreaseNumberOfCalculatingRSThreads(void) {
  CSingleLock singleLock(&m_NumberOfCalculatingRSThreads);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    ASSERT(m_iNumberOfCalculatingRSThreads > 0);
    m_iNumberOfCalculatingRSThreads--;
    singleLock.Unlock();
  }
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// �ж��Ƿ��п�����߳̿ɹ�ʹ�á�
//
// Ŀǰ����16�߳��ƣ������������16�������̡߳�
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CThreadStatus::IsCalculatingRSThreadAvailable(void) {
  CSingleLock singleLock(&m_NumberOfCalculatingRSThreads);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    bool fFlag;
    if (m_iNumberOfCalculatingRSThreads >= gl_cMaxCalculatingRSThreads) { // �������8���߳�ͬʱ���С�������߳����׵���ϵͳ��Ӧ��������û��Ҫ��
      fFlag = false;
    }
    else fFlag = true;
    singleLock.Unlock();
    return fFlag;
  }
  ASSERT(0);
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
}

bool CThreadStatus::IsCalculatingRSThreadRunning(void) {
  CSingleLock singleLock(&m_NumberOfCalculatingRSThreads);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    bool fFlag;
    if (m_iNumberOfCalculatingRSThreads > 0) {
      fFlag = true;
    }
    else fFlag = false;
    singleLock.Unlock();

    return fFlag;
  }
  ASSERT(0);
  return false; // �˷�֧������ִ�е���ֻΪ�������������ľ��������
}