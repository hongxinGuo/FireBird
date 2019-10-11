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

  m_fExitingThread = false;

  m_fReadingNeteaseDayLine = false;
  m_fSavingDayLine = false;
  m_fReadingSinaRTData = false;
  m_fSinaRTDataReceived = false;
  m_fRTDataNeedCalculate = false;
  m_fDayLineDataReady = false;
  m_fCalculatingRTData = false;
  m_fSavingTempData = false;
  m_fSavingStockCodeData = false;

  m_iNumberOfCalculatingRSThreads = 0;
}

void CThreadStatus::SetExitingThread(bool fFlag) {
  CSingleLock singleLock(&m_ExitingThreadLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fExitingThread = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsExitingThread(void) {
  CSingleLock singleLock(&m_ExitingThreadLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fExitingThread;
    singleLock.Unlock();
    return fFlag;
  }
  return false; // �˷�֧������ִ�е�
}

void CThreadStatus::SetCalculatingDayLineRS(bool fFlag) {
  CSingleLock singleLock(&m_CalculatingDayLineRSLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fCalculatingDayLineRelativeStrong = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsCalculatingDayLineRS(void) {
  CSingleLock singleLock(&m_CalculatingDayLineRSLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fCalculatingDayLineRelativeStrong;
    singleLock.Unlock();
    return fFlag;
  }
  return false; // �˷�֧������ִ�е�
}

void CThreadStatus::SetReadingNeteaseDayLine(bool fFlag) {
  CSingleLock singleLock(&m_ReadingNeteaseDayLineLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fReadingNeteaseDayLine = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsReadingNeteaseDayLine(void) {
  CSingleLock singleLock(&m_ReadingNeteaseDayLineLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fReadingNeteaseDayLine;
    singleLock.Unlock();
    return fFlag;
  }
  return false; // �˷�֧������ִ�е�
}

void CThreadStatus::SetSavingDayLine(bool fFlag) {
  CSingleLock singleLock(&m_SavingDayLineLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fSavingDayLine = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsSavingDayLine(void) {
  CSingleLock singleLock(&m_SavingDayLineLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fSavingDayLine;
    singleLock.Unlock();
    return fFlag;
  }
  return false; // �˷�֧������ִ�е�
}

void CThreadStatus::SetReadingSinaRTData(bool fFlag) {
  CSingleLock singleLock(&m_ReadingSinaRTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fReadingSinaRTData = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsReadingSinaRTData(void) {
  CSingleLock singleLock(&m_ReadingSinaRTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fReadingSinaRTData;
    singleLock.Unlock();
    return fFlag;
  }
  return false; // �˷�֧������ִ�е�
}

void CThreadStatus::SetReadingTengxunRTData(bool fFlag) {
  CSingleLock singleLock(&m_ReadingTengxunRTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fReadingTengxunRTData = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsReadingTengxunRTData(void) {
  CSingleLock singleLock(&m_ReadingTengxunRTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fReadingTengxunRTData;
    singleLock.Unlock();
    return fFlag;
  }
  return false; // �˷�֧������ִ�е�
}

void CThreadStatus::SetSinaRTDataReceived(bool fFlag) {
  CSingleLock singleLock(&m_SinaRTDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fSinaRTDataReceived = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsSinaRTDataReceived(void) {
  CSingleLock singleLock(&m_SinaRTDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fSinaRTDataReceived;
    singleLock.Unlock();
    return fFlag;
  }
  return false; // �˷�֧������ִ�е�
}

void CThreadStatus::SetTengxunRTDataReceived(bool fFlag) {
  CSingleLock singleLock(&m_TengxunRTDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fTengxunRTDataReceived = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsTengxunRTDataReceived(void) {
  CSingleLock singleLock(&m_TengxunRTDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fTengxunRTDataReceived;
    singleLock.Unlock();
    return fFlag;
  }
  return false; // �˷�֧������ִ�е�
}

void CThreadStatus::SetRTDataNeedCalculate(bool fFlag) {
  CSingleLock singleLock(&m_RTDataNeedCalculateLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fRTDataNeedCalculate = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsRTDataNeedCalculate(void) {
  CSingleLock singleLock(&m_RTDataNeedCalculateLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fRTDataNeedCalculate;
    singleLock.Unlock();
    return fFlag;
  }
  return false; // �˷�֧������ִ�е�

}

void CThreadStatus::SetDayLineDataReady(bool fFlag) {
  CSingleLock singleLock(&m_DayLineDataReadyLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fDayLineDataReady = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsDayLineDataReady(void) {
  CSingleLock singleLock(&m_DayLineDataReadyLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fDayLineDataReady;
    singleLock.Unlock();
    return fFlag;
  }
  return false; // �˷�֧������ִ�е�
}

void CThreadStatus::SetCalculatingRTData(bool fFlag) {
  CSingleLock singleLock(&m_CalculatingRTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fCalculatingRTData = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsCalculatingRTData(void) {
  CSingleLock singleLock(&m_CalculatingRTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fCalculatingRTData;
    singleLock.Unlock();
    return fFlag;
  }
  return false; // �˷�֧������ִ�е�
}

void CThreadStatus::SetSavingTempData(bool fFlag) {
  CSingleLock singleLock(&m_SavingTempDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fSavingTempData = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsSavingTempData(void) {
  CSingleLock singleLock(&m_SavingTempDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fSavingTempData;
    singleLock.Unlock();
    return fFlag;
  }
  return false; // �˷�֧������ִ�е�
}

void CThreadStatus::SetSavingStockCodeData(bool fFlag) {
  CSingleLock singleLock(&m_SavingStockCodeDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fSavingStockCodeData = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsSavingStockCodeData(void) {
  CSingleLock singleLock(&m_SavingStockCodeDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fSavingStockCodeData;
    singleLock.Unlock();
    return fFlag;
  }
  return false; // �˷�֧������ִ�е�
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
  return false; // �˷�֧������ִ�е�
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
  return false; // �˷�֧������ִ�е�
}
