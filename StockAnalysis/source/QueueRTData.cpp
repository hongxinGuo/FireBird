#include"globedef.h"

#include "QueueRTData.h"
#include "SystemMessage.h"

CQueueRTData::CQueueRTData() {
}

CQueueRTData::~CQueueRTData() {
}

void CQueueRTData::Reset(void) {
  long lTotal = m_priorityqueueRTStockData.size();
  for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
    m_priorityqueueRTStockData.pop();
  }
}

void CQueueRTData::PushRTData(CRTDataPtr pData) {
  m_priorityqueueRTStockData.push(pData);
}

CRTDataPtr CQueueRTData::PopRTData(void) {
  CRTDataPtr pData;
  pData = m_priorityqueueRTStockData.top();
  m_priorityqueueRTStockData.pop();
  return pData;
}

long CQueueRTData::GetRTDataSize(void) {
  return(m_priorityqueueRTStockData.size());
}