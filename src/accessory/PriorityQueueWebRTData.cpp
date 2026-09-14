#include"pch.h"

#include"PriorityQueueWebRTData.h"

#include "WebRTData.h"

void CPriorityQueueWebRTData::Reset() {
	m_MutexAccessData.lock();
	const size_t lTotal = m_priorityQueueWebRTData.size();
	for (size_t i = 0; i < lTotal; i++) {
		// 清空队列
		m_priorityQueueWebRTData.pop();
	}
	m_MutexAccessData.unlock();
}

void CPriorityQueueWebRTData::PushData(const CWebRTDataPtr& pData) {
	m_MutexAccessData.lock();
	m_priorityQueueWebRTData.push(pData);
	m_MutexAccessData.unlock();
}

CWebRTDataPtr CPriorityQueueWebRTData::PopData() {
	m_MutexAccessData.lock();
	CWebRTDataPtr pData = m_priorityQueueWebRTData.top();
	m_priorityQueueWebRTData.pop();
	m_MutexAccessData.unlock();
	return pData;
}

size_t CPriorityQueueWebRTData::Size() {
	m_MutexAccessData.lock();
	size_t const size = m_priorityQueueWebRTData.size();
	m_MutexAccessData.unlock();
	return size;
}
bool CPriorityQueueWebRTData::cmpRTData::operator()(const shared_ptr<CWebRTData>& p1, const shared_ptr<CWebRTData>& p2) const {
	// 有优先级的队列默认排列顺序是从大到小，故而从小到大排列需要使用 > 符号。
	// 相同时间的数据，先放入的数据位于后放入的数据前面。
	// 不同时间的数据，时间较早的数据位于时间较晚的数据前面。
	return (p1->GetTime() > p2->GetTime());
}
