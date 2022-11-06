///////////////////////////////////////////////////////////////////////////
//
// C++11��û���ź������ʶ�����mutex��condition_variable��׼�⹹��Semaphore
// �����ӿڣ�
// Wait() : P������������Դ
// Signal() : V�������ͷ���Դ
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

		void SetMaxCount(long lCount = 1) { ASSERT(lCount >= 1);  m_count = lCount; } // ��󹲴���������С��1
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
