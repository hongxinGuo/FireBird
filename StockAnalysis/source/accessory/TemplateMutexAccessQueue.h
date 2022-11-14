///////////////////////////////////////////////////////////
//
// 单向队列，用于存储从网络上提取的数据。由于提取数据采用线程模式，
// 故而此队列需要使用互斥区，以防止发生同步问题。
//
// 队列中存储的为一个指向具体数据T的智能指针，
//
///////////////////////////////////////////////////////////
#pragma once

using namespace std;
#include<queue>
#include<mutex>
#include<memory>

template <typename T> class CTemplateMutexAccessQueue {
public:
	CTemplateMutexAccessQueue() {}
	~CTemplateMutexAccessQueue() {}
	void Reset(void) {
		m_MutexAccessData.lock();
		const auto lTotal = m_qData.size();
		for (int i = 0; i < lTotal; i++) {
			m_qData.pop();
		}
		m_MutexAccessData.unlock();
	}

	// 通用接口函数
	void PushData(shared_ptr<T> pData) {
		m_MutexAccessData.lock();
		m_qData.push(pData);
		m_MutexAccessData.unlock();
	}

	shared_ptr<T> PopData(void) {
		ASSERT(m_qData.size() > 0);
		m_MutexAccessData.lock();
		shared_ptr<T> pData = m_qData.front();
		m_qData.pop();
		m_MutexAccessData.unlock();
		return pData;
	}

	size_t Size(void) {
		m_MutexAccessData.lock();
		size_t size = m_qData.size();
		m_MutexAccessData.unlock();
		return size;
	}

protected:
	queue<shared_ptr<T>> m_qData;
	mutex m_MutexAccessData;
};
