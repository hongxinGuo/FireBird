#include"stdafx.h"

#include"SystemStatus.h"

CSystemStatus::CSystemStatus() {
  m_fReadingInProcess = false;
  m_fDataBaseInProcess = false;
  m_fCalculatingRelativeStrongInProcess = false;
  m_fRTDataReadingInProcess = false;
  m_fRTDataReady = false;
  m_fDayLineDataReady = false;
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
    bool fFlag = m_fCalculatingRelativeStrongInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CSystemStatus::SetReadingInProcess(bool fFlag) {
  CSingleLock singleLock(&m_ReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fReadingInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CSystemStatus::IsReadingInProcess(void) {
  CSingleLock singleLock(&m_ReadingInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    bool fFlag = m_fReadingInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CSystemStatus::SetDataBaseInProcess(bool fFlag) {
  CSingleLock singleLock(&m_DataBaseInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fDataBaseInProcess = fFlag;
    singleLock.Unlock();
  }
}

bool CSystemStatus::IsDataBaseInProcess(void) {
  CSingleLock singleLock(&m_DataBaseInProcessLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    bool fFlag = m_fDataBaseInProcess;
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
    bool fFlag = m_fRTDataReadingInProcess;
    singleLock.Unlock();
    return fFlag;
  }
}

void CSystemStatus::SetRTDataReady(bool fFlag) {
  CSingleLock singleLock(&m_RTDataReadyLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fRTDataReady = fFlag;
    singleLock.Unlock();
  }
}

bool CSystemStatus::IsRTDataReady(void) {
  CSingleLock singleLock(&m_RTDataReadyLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    bool fFlag = m_fRTDataReady;
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
    bool fFlag = m_fDayLineDataReady;
    singleLock.Unlock();
    return fFlag;
  }
}
