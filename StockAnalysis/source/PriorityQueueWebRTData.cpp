#include"pch.h"
#include"globedef.h"

#include"PriorityQueueWebRTData.h"

CPriorityQueueWebRTData::CPriorityQueueWebRTData() {
}

CPriorityQueueWebRTData::~CPriorityQueueWebRTData() {
}

void CPriorityQueueWebRTData::Reset(void) {
	m_MutexAccessData.lock();
	const size_t lTotal = m_priorityqueueStockData.size();
	for (int i = 0; i < lTotal; i++) { // Çå¿Õ¶ÓÁÐ
		m_priorityqueueStockData.pop();
	}
	m_MutexAccessData.unlock();
}

void CPriorityQueueWebRTData::PushData(CWebRTDataPtr pData) {
	m_MutexAccessData.lock();
	m_priorityqueueStockData.push(pData);
	m_MutexAccessData.unlock();
}

CWebRTDataPtr CPriorityQueueWebRTData::PopData(void) {
	CWebRTDataPtr pData;
	m_MutexAccessData.lock();
	pData = m_priorityqueueStockData.top();
	m_priorityqueueStockData.pop();
	m_MutexAccessData.unlock();
	return pData;
}

size_t CPriorityQueueWebRTData::GetDataSize(void) {
	size_t size = 0;
	m_MutexAccessData.lock();
	size = m_priorityqueueStockData.size();
	m_MutexAccessData.unlock();
	return size;
}