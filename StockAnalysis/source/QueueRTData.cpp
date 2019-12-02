#include"globedef.h"

#include "QueueRTData.h"
#include "SystemMessage.h"

CQueueRTData::CQueueRTData() {
}

CQueueRTData::~CQueueRTData() {
}

void CQueueRTData::Reset(void) {
  CSingleLock singleLock(&m_cs);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lTotal = m_queueRTStockData.size();
    for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
      m_queueRTStockData.pop();
    }
    singleLock.Unlock();
  }
}

void CQueueRTData::PushRTData(CRTDataPtr pData) {
  CSingleLock singleLock(&m_cs);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_queueRTStockData.push(pData);
    singleLock.Unlock();
  }
}

CRTDataPtr CQueueRTData::PopRTData(void) {
  CRTDataPtr pData;
  CSingleLock singleLock(&m_cs);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    pData = m_queueRTStockData.front();
    m_queueRTStockData.pop();
    singleLock.Unlock();
    return pData;
  }
}

long CQueueRTData::GetRTDataSize(void) {
  size_t size = 0;
  CSingleLock singleLock(&m_cs);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    size = m_queueRTStockData.size();
    singleLock.Unlock();
    return size;
  }
}