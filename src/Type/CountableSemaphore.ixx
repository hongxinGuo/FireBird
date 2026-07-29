module;

export module CounableSemaphore;

import std;
using std::counting_semaphore;

export {
	constexpr int MAX_BACKGROUND_WORKING_THREAD_ = 32;

	class CCountableSemaphore {
	public:
		CCountableSemaphore() : m_semaphore(MAX_BACKGROUND_WORKING_THREAD_), m_count(0) {}
		~CCountableSemaphore() {}
		void AcquireWithoutCount() {
			m_semaphore.acquire();
		}
		void ReleaseWithoutCount() {
			m_semaphore.release();
		}

		void Acquire() {
			m_semaphore.acquire();
			++m_count;
		}
		void Release() {
			m_semaphore.release();
			--m_count;
		}

		int GetCount() const noexcept {
			return m_count.load();
		}

	protected:
		std::counting_semaphore<MAX_BACKGROUND_WORKING_THREAD_> m_semaphore;
		std::atomic_int m_count;
	};

	extern CCountableSemaphore gl_BackgroundWorkingThread; // 最多后台工作线程允许数量
}