#include"stdafx.h"

#include"globedef.h"

#include"SystemStatus.h"
#include"SystemMessage.h"

CSystemStatus::CSystemStatus() {
  static int siInstance = 0;
  if (siInstance++ > 0) {
    TRACE("系统状态只允许生成一个实例\n");
    gl_systemMessage.PushInformationMessage(_T("系统状态只允许生成一个实例"));
  }
  
  m_fExitingClientThreadInProcess = false;

  m_fDayLineReadingInProcess = false;
  m_fSavingDayLineInProcess = false;
  m_fCalculatingRelativeStrongInProcess = false;
  m_fRTDataReadingInProcess = false;
  m_fRTDataReceived = false;
  m_fRTDataNeedCalculate = false;
  m_fDayLineDataReady = false;
  m_fCalculatingRTData = false;
  m_fSavingTempData = false; 
  m_fSavingStockCodeData = false;

}

void CSystemStatus::SetExitingClientThreadInProcess(bool fFlag) {
  CSingleLock singleLock(&m_ExitingClientThreadInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fExitingClientThreadInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CSystemStatus::IsExitingClientThreadInProcess(void) {
  CSingleLock singleLock(&m_ExitingClientThreadInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fExitingClientThreadInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CSystemStatus::SetCalculateRSInProcess(bool fFlag) {
  CSingleLock singleLock(&m_CalculateRSInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fCalculatingRelativeStrongInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CSystemStatus::IsCalculateRSInProcess(void) {
  CSingleLock singleLock(&m_CalculateRSInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fCalculatingRelativeStrongInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CSystemStatus::SetDayLineReadingInProcess(bool fFlag) {
  CSingleLock singleLock(&m_DayLineReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fDayLineReadingInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CSystemStatus::IsDayLineReadingInProcess(void) {
  CSingleLock singleLock(&m_DayLineReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fDayLineReadingInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CSystemStatus::SetSavingDayLineInProcess(bool fFlag) {
  CSingleLock singleLock(&m_SavingDayLineInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fSavingDayLineInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CSystemStatus::IsSavingDayLineInProcess(void) {
  CSingleLock singleLock(&m_SavingDayLineInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fSavingDayLineInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CSystemStatus::SetRTDataReadingInProcess(bool fFlag) {
  CSingleLock singleLock(&m_RTDataReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fRTDataReadingInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CSystemStatus::IsRTDataReadingInProcess(void) {
  CSingleLock singleLock(&m_RTDataReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fRTDataReadingInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CSystemStatus::SetRTDataReceived(bool fFlag) {
  CSingleLock singleLock(&m_RTDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fRTDataReceived = fFlag;
    singleLock.Unlock();
  }
}

bool CSystemStatus::IsRTDataReceived(void) {
  CSingleLock singleLock(&m_RTDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fRTDataReceived;
    singleLock.Unlock();
    return fFlag;
  }
}

void CSystemStatus::SetRTDataNeedCalculate(bool fFlag) {
  CSingleLock singleLock(&m_RTDataNeedCalculateLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fRTDataNeedCalculate = fFlag;
    singleLock.Unlock();
  }
}

bool CSystemStatus::IsRTDataNeedCalculate(void) {
  CSingleLock singleLock(&m_RTDataNeedCalculateLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fRTDataNeedCalculate;
    singleLock.Unlock();
    return fFlag;
  }
}

void CSystemStatus::SetDayLineDataReady(bool fFlag) {
  CSingleLock singleLock(&m_DayLineDataReadyLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fDayLineDataReady = fFlag;
    singleLock.Unlock();
  }
}

bool CSystemStatus::IsDayLineDataReady(void) {
  CSingleLock singleLock(&m_DayLineDataReadyLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fDayLineDataReady;
    singleLock.Unlock();
    return fFlag;
  }
}

void CSystemStatus::SetCalculatingRTData(bool fFlag) {
  CSingleLock singleLock(&m_CalculatingRTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fCalculatingRTData = fFlag;
    singleLock.Unlock();
  }
}

bool CSystemStatus::IsCalculatingRTData(void) {
  CSingleLock singleLock(&m_CalculatingRTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fCalculatingRTData;
    singleLock.Unlock();
    return fFlag;
  }
}

void CSystemStatus::SetSavingTempData(bool fFlag) {
  CSingleLock singleLock(&m_SavingTempDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fSavingTempData = fFlag;
    singleLock.Unlock();
  }
}

bool CSystemStatus::IsSavingTempData(void) {
  CSingleLock singleLock(&m_SavingTempDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fSavingTempData;
    singleLock.Unlock();
    return fFlag;
  }
}

void CSystemStatus::SetSavingStockCodeData(bool fFlag) {
  CSingleLock singleLock(&m_SavingStockCodeDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fSavingStockCodeData = fFlag;
    singleLock.Unlock();
  }
}

bool CSystemStatus::IsSavingStockCodeData(void) {
  CSingleLock singleLock(&m_SavingStockCodeDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fSavingStockCodeData;
    singleLock.Unlock();
    return fFlag;
  }
}
