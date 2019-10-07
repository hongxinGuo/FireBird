#include"stdafx.h"

#include"globedef.h"

#include"ThreadStatus.h"
#include"SystemMessage.h"

CThreadStutus::CThreadStutus() {
  static int siInstance = 0;
  if (siInstance++ > 0) {
    TRACE("系统状态只允许生成一个实例\n");
    gl_systemMessage.PushInformationMessage(_T("系统状态只允许生成一个实例"));
  }

  m_fExitingClientThreadInProcess = false;

  m_fNeteaseDayLineReadingInProcess = false;
  m_fSavingDayLineInProcess = false;
  m_fCalculatingRelativeStrongInProcess = false;
  m_fSinaRTDataReadingInProcess = false;
  m_fSinaRTDataReceived = false;
  m_fRTDataNeedCalculate = false;
  m_fDayLineDataReady = false;
  m_fCalculatingRTData = false;
  m_fSavingTempData = false;
  m_fSavingStockCodeData = false;

  m_iNumberOfCalculatingRSThreads = 0;
}

void CThreadStutus::SetExitingClientThreadInProcess(bool fFlag) {
  CSingleLock singleLock(&m_ExitingClientThreadInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fExitingClientThreadInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsExitingClientThreadInProcess(void) {
  CSingleLock singleLock(&m_ExitingClientThreadInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fExitingClientThreadInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStutus::SetCalculateRSInProcess(bool fFlag) {
  CSingleLock singleLock(&m_CalculateRSInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fCalculatingRelativeStrongInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsCalculateRSInProcess(void) {
  CSingleLock singleLock(&m_CalculateRSInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fCalculatingRelativeStrongInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStutus::SetCalculateDayLineRS(bool fFlag) {
  CSingleLock singleLock(&m_CalculateDayLineRSLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fCalculatingDayLineRelativeStrong = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsCalculateDayLineRS(void) {
  CSingleLock singleLock(&m_CalculateDayLineRSLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fCalculatingDayLineRelativeStrong;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStutus::SetNeteaseDayLineReadingInProcess(bool fFlag) {
  CSingleLock singleLock(&m_NeteaseDayLineReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fNeteaseDayLineReadingInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsNeteaseDayLineReadingInProcess(void) {
  CSingleLock singleLock(&m_NeteaseDayLineReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fNeteaseDayLineReadingInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStutus::SetSavingDayLineInProcess(bool fFlag) {
  CSingleLock singleLock(&m_SavingDayLineInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fSavingDayLineInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsSavingDayLineInProcess(void) {
  CSingleLock singleLock(&m_SavingDayLineInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fSavingDayLineInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStutus::SetSinaRTDataReadingInProcess(bool fFlag) {
  CSingleLock singleLock(&m_SinaRTDataReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fSinaRTDataReadingInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsSinaRTDataReadingInProcess(void) {
  CSingleLock singleLock(&m_SinaRTDataReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fSinaRTDataReadingInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStutus::SetTengxunRTDataReadingInProcess(bool fFlag) {
  CSingleLock singleLock(&m_TengxunRTDataReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fTengxunRTDataReadingInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsTengxunRTDataReadingInProcess(void) {
  CSingleLock singleLock(&m_TengxunRTDataReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fTengxunRTDataReadingInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStutus::SetSinaRTDataReceived(bool fFlag) {
  CSingleLock singleLock(&m_SinaRTDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fSinaRTDataReceived = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsSinaRTDataReceived(void) {
  CSingleLock singleLock(&m_SinaRTDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fSinaRTDataReceived;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStutus::SetTengxunRTDataReceived(bool fFlag) {
  CSingleLock singleLock(&m_TengxunRTDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fTengxunRTDataReceived = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsTengxunRTDataReceived(void) {
  CSingleLock singleLock(&m_TengxunRTDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fTengxunRTDataReceived;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStutus::SetRTDataNeedCalculate(bool fFlag) {
  CSingleLock singleLock(&m_RTDataNeedCalculateLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fRTDataNeedCalculate = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsRTDataNeedCalculate(void) {
  CSingleLock singleLock(&m_RTDataNeedCalculateLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fRTDataNeedCalculate;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStutus::SetDayLineDataReady(bool fFlag) {
  CSingleLock singleLock(&m_DayLineDataReadyLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fDayLineDataReady = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsDayLineDataReady(void) {
  CSingleLock singleLock(&m_DayLineDataReadyLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fDayLineDataReady;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStutus::SetCalculatingRTData(bool fFlag) {
  CSingleLock singleLock(&m_CalculatingRTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fCalculatingRTData = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsCalculatingRTData(void) {
  CSingleLock singleLock(&m_CalculatingRTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fCalculatingRTData;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStutus::SetSavingTempData(bool fFlag) {
  CSingleLock singleLock(&m_SavingTempDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fSavingTempData = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsSavingTempData(void) {
  CSingleLock singleLock(&m_SavingTempDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fSavingTempData;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStutus::SetSavingStockCodeData(bool fFlag) {
  CSingleLock singleLock(&m_SavingStockCodeDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fSavingStockCodeData = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsSavingStockCodeData(void) {
  CSingleLock singleLock(&m_SavingStockCodeDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fSavingStockCodeData;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStutus::IncreaseNunberOfCalculatingRSThreads(void) {
  CSingleLock singleLock(&m_NumberOfCalculatingRSThreads);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_iNumberOfCalculatingRSThreads++;
    singleLock.Unlock();
  }
}

void CThreadStutus::DecreaseNumberOfCalculatingRSThreads(void) {
  CSingleLock singleLock(&m_NumberOfCalculatingRSThreads);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_iNumberOfCalculatingRSThreads--;
    singleLock.Unlock();
  }
}

bool CThreadStutus::IsCalculatingRSThreadAvailable(void) {
  CSingleLock singleLock(&m_NumberOfCalculatingRSThreads);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    bool fFlag;
    if (m_iNumberOfCalculatingRSThreads >= 8) { // 最多允许8个线程同时运行。更多的线程容易导致系统响应变慢
      fFlag = false;
    }
    else fFlag = true;
    singleLock.Unlock();
    return fFlag;
  }
}