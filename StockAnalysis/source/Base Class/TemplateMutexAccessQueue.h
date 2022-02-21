///////////////////////////////////////////////////////////
//
// 单向队列，用于存储从网络上提取的数据。由于提取数据采用线程模式，
// 故而此队列需要使用互斥区，以防止发生同步问题。
//
///////////////////////////////////////////////////////////
#pragma once

using namespace std;
#include<queue>
#include<mutex>

template <typename T> class CTemplateMutexAccessQueue {
public:
	CTemplateMutexAccessQueue() {}
	~CTemplateMutexAccessQueue() {}
	void Reset(void) { m_MutexAccessData.lock(); const auto lTotal = m_qData.size(); for (int i = 0; i < lTotal; i++) { m_qData.pop(); }	m_MutexAccessData.unlock(); }

	// 通用接口函数
	void PushData(T data) { m_MutexAccessData.lock(); m_qData.push(data); m_MutexAccessData.unlock(); }
	T PopData(void) { T data;	m_MutexAccessData.lock();	data = m_qData.front(); m_qData.pop();	m_MutexAccessData.unlock();	return data; }
	size_t GetDataSize(void) { size_t size = 0;	m_MutexAccessData.lock();	size = m_qData.size(); m_MutexAccessData.unlock(); return size; }

protected:
	queue<T> m_qData;
	mutex m_MutexAccessData;
};
