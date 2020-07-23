#include"globedef.h"

CPriorityQueueRTData::CPriorityQueueRTData() {
}

CPriorityQueueRTData::~CPriorityQueueRTData() {
}

void CPriorityQueueRTData::Reset(void) {
  size_t lTotal = m_priorityqueueRTStockData.size();
  for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
    m_priorityqueueRTStockData.pop();
  }
}

void CPriorityQueueRTData::PushRTData(CRTDataPtr pData) {
  m_priorityqueueRTStockData.push(pData);
}

CRTDataPtr CPriorityQueueRTData::PopRTData(void) {
  CRTDataPtr pData;
  pData = m_priorityqueueRTStockData.top();
  m_priorityqueueRTStockData.pop();
  return pData;
}

size_t CPriorityQueueRTData::GetRTDataSize(void) {
  return(m_priorityqueueRTStockData.size());
}