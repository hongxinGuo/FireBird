///////////////////////////////////////////////////////////////////////////
//
// C++11中没有信号量，故而采用mutex和condition_variable标准库构造Semaphore
// 公共接口：
// Wait() : P操作，申请资源
// Signal() : V操作，释放资源
//
///////////////////////////////////////////////////////////////////////////
#pragma once

using namespace std;
#include<mutex>
#include<condition_variable>

namespace MyLib {
	class Semaphore
	{
	public:
		Semaphore(long count = 1) noexcept : m_count(count) {}
		Semaphore(const Semaphore&) = delete;
		Semaphore& operator=(const Semaphore&) = delete;

		void SetMaxCount(long lCount = 1) { ASSERT(lCount >= 1);  m_count = lCount; } // 最大共存数不允许小于1
		long GetMaxCount(void) noexcept { return m_count; }

		void Signal() {
			{
				unique_lock<mutex> lock(m_mutex);
				++m_count;
			}
			m_cv_uptr.notify_one();
		}

		void Wait() {
			unique_lock<mutex> lock(m_mutex);
			while (m_count < 1) { // we may have spurious wakeup!
				m_cv_uptr.wait(lock);
			}
			--m_count;
		}

	private:
		mutex m_mutex;
		condition_variable m_cv_uptr;
		long m_count;
	};
}
