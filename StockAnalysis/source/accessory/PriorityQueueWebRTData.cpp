#include"pch.h"

#include"PriorityQueueWebRTData.h"

CPriorityQueueWebRTData::CPriorityQueueWebRTData() {
}

CPriorityQueueWebRTData::~CPriorityQueueWebRTData() {
}

void CPriorityQueueWebRTData::Reset(void) {
	m_MutexAccessData.lock();
	const size_t lTotal = m_priorityQueueWebRTData.size();
	for (int i = 0; i < lTotal; i++) {
		// Çå¿Õ¶ÓÁÐ
		m_priorityQueueWebRTData.pop();
	}
	m_MutexAccessData.unlock();
}

void CPriorityQueueWebRTData::PushData(CWebRTDataPtr pData) {
	m_MutexAccessData.lock();
	m_priorityQueueWebRTData.push(pData);
	m_MutexAccessData.unlock();
}

CWebRTDataPtr CPriorityQueueWebRTData::PopData(void) {
	m_MutexAccessData.lock();
	CWebRTDataPtr pData = m_priorityQueueWebRTData.top();
	m_priorityQueueWebRTData.pop();
	m_MutexAccessData.unlock();
	return pData;
}

size_t CPriorityQueueWebRTData::Size(void) {
	m_MutexAccessData.lock();
	size_t const size = m_priorityQueueWebRTData.size();
	m_MutexAccessData.unlock();
	return size;
}
