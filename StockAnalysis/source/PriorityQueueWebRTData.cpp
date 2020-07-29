#include"globedef.h"

#include"PriorityQueueWebRTData.h"

CPriorityQueueWebRTData::CPriorityQueueWebRTData() {
}

CPriorityQueueWebRTData::~CPriorityQueueWebRTData() {
}

void CPriorityQueueWebRTData::Reset(void) {
  size_t lTotal = m_priorityqueueStockData.size();
  for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
    m_priorityqueueStockData.pop();
  }
}

void CPriorityQueueWebRTData::PushData(CWebRTDataPtr pData) {
  m_priorityqueueStockData.push(pData);
}

CWebRTDataPtr CPriorityQueueWebRTData::PopData(void) {
  CWebRTDataPtr pData;
  pData = m_priorityqueueStockData.top();
  m_priorityqueueStockData.pop();
  return pData;
}

size_t CPriorityQueueWebRTData::GetDataSize(void) {
  return(m_priorityqueueStockData.size());
}