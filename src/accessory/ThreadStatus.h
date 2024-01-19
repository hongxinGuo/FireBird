#pragma once

#include<semaphore>
#include<atomic>

using std::counting_semaphore;
using std::atomic_int;

class CThreadStatus final {
	//各线程状态
public:
	CThreadStatus();
	//只能有一个实例,不允许赋值。
	CThreadStatus(const CThreadStatus&) = delete;
	CThreadStatus& operator=(const CThreadStatus&) = delete;
	CThreadStatus(const CThreadStatus&&) noexcept = delete;
	CThreadStatus& operator=(const CThreadStatus&&) noexcept = delete;
	~CThreadStatus() = default;

	//并发执行计算日线相对强度的计数器，最多允许cMaxBackGroundTaskThreads个线程同时执行
	void IncreaseBackGroundWorkingThread() noexcept { ++m_NumberOfBackGroundWorkingThreads; } //同时运行线程数加一
	void DecreaseBackGroundWorkingThread() noexcept { --m_NumberOfBackGroundWorkingThreads; } //同时运行线程数减一
	[[nodiscard]] bool IsBackGroundThreadsWorking() const noexcept { return m_NumberOfBackGroundWorkingThreads > 0; } //计算日线的线程是否处于运行中
	[[nodiscard]] int GetNumberOfBackGroundWorkingThread() const noexcept { return m_NumberOfBackGroundWorkingThreads; }

	[[nodiscard]] static bool IsSavingThreadRunning() noexcept;

	void IncreaseWebInquiringThread() noexcept { ++m_NumberOfWebInquiringThread; }
	void DecreaseWebInquiringThread() noexcept {
		ASSERT(m_NumberOfWebInquiringThread >= 0);
		--m_NumberOfWebInquiringThread;
	}
	int GetNumberOfWebInquiringThread() const noexcept { return m_NumberOfWebInquiringThread; }
	[[nodiscard]] bool IsWebInquiringThreadRunning() const noexcept { return m_NumberOfWebInquiringThread > 0; }

protected:
	atomic_int m_NumberOfBackGroundWorkingThreads; //正在计算日线相对强度的线程数。目前最多同时允许BackGroundThreadPermittedNumber个线程
	atomic_int m_NumberOfWebInquiringThread; //正在运行的提取网络数据工作线程数量
};

extern CThreadStatus gl_ThreadStatus; //系统中的各种状态，被各个工作线程所使用
