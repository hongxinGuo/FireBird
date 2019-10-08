#include"stdafx.h"

#include"globedef.h"

#include"ThreadStatus.h"
#include"SystemMessage.h"

CThreadStatus::CThreadStatus() {
  static int siInstance = 0;
  if (siInstance++ > 0) {
    TRACE("系统状态只允许生成一个实例\n");
    gl_systemMessage.PushInformationMessage(_T("系统状态只允许生成一个实例"));
  }

  m_fExitingThreadInProcess = false;

  m_fNeteaseDayLineReadingInProcess = false;
  m_fSavingDayLineInProcess = false;
  m_fSinaRTDataReadingInProcess = false;
  m_fSinaRTDataReceived = false;
  m_fRTDataNeedCalculate = false;
  m_fDayLineDataReady = false;
  m_fCalculatingRTData = false;
  m_fSavingTempData = false;
  m_fSavingStockCodeData = false;

  m_iNumberOfCalculatingRSThreads = 0;
}

void CThreadStatus::SetExitingThreadInProcess(bool fFlag) {
  CSingleLock singleLock(&m_ExitingThreadInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fExitingThreadInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsExitingThreadInProcess(void) {
  CSingleLock singleLock(&m_ExitingThreadInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fExitingThreadInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStatus::SetCalculateDayLineRS(bool fFlag) {
  CSingleLock singleLock(&m_CalculateDayLineRSLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fCalculatingDayLineRelativeStrong = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsCalculateDayLineRS(void) {
  CSingleLock singleLock(&m_CalculateDayLineRSLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fCalculatingDayLineRelativeStrong;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStatus::SetNeteaseDayLineReadingInProcess(bool fFlag) {
  CSingleLock singleLock(&m_NeteaseDayLineReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fNeteaseDayLineReadingInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsNeteaseDayLineReadingInProcess(void) {
  CSingleLock singleLock(&m_NeteaseDayLineReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fNeteaseDayLineReadingInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStatus::SetSavingDayLineInProcess(bool fFlag) {
  CSingleLock singleLock(&m_SavingDayLineInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fSavingDayLineInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsSavingDayLineInProcess(void) {
  CSingleLock singleLock(&m_SavingDayLineInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fSavingDayLineInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStatus::SetSinaRTDataReadingInProcess(bool fFlag) {
  CSingleLock singleLock(&m_SinaRTDataReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fSinaRTDataReadingInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsSinaRTDataReadingInProcess(void) {
  CSingleLock singleLock(&m_SinaRTDataReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fSinaRTDataReadingInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CThreadStatus::SetTengxunRTDataReadingInProcess(bool fFlag) {
  CSingleLock singleLock(&m_TengxunRTDataReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fTengxunRTDataReadingInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CThreadStatus::IsTengxunRTDataReadingInProcess(void) {
  CSingleLock singleLock(&m_TengxunRTDataReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fTengxunRTDataReadingInProcess;
    singleLock.Unlock();
    return fFlag;
  }
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
}

void CThreadStatus::IncreaseNunberOfCalculatingRSThreads(void) {
  CSingleLock singleLock(&m_NumberOfCalculatingRSThreads);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    ASSERT(m_iNumberOfCalculatingRSThreads < 16);
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
// 判断是否还有空余的线程可供使用。
//
// 目前采用16线程制，允许最多生成16个工作线程。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CThreadStatus::IsCalculatingRSThreadAvailable(void) {
  CSingleLock singleLock(&m_NumberOfCalculatingRSThreads);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    bool fFlag;
    if (m_iNumberOfCalculatingRSThreads >= 8) { // 最多允许8个线程同时运行。更多的线程容易导致系统响应变慢，且没必要。
      fFlag = false;
    }
    else fFlag = true;
    singleLock.Unlock();
    return fFlag;
  }
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
}
