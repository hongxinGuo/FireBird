#pragma once

using namespace std;
#include<atomic>
#include<semaphore>

extern counting_semaphore<4> gl_SaveDayLineThreadPermitted; // 默认为1.当数据库中没有日线数据时，增加此信号量最大值至4

class CThreadStatus  final { // 各线程状态
public:
	CThreadStatus();
	// 只能有一个实例,不允许赋值。
	CThreadStatus(const CThreadStatus&) = delete;
	CThreadStatus& operator=(const CThreadStatus&) = delete;
	CThreadStatus(const CThreadStatus&&) noexcept = delete;
	CThreadStatus& operator=(const CThreadStatus&&) noexcept = delete;
	~CThreadStatus();

	// 并发执行计算日线相对强度的计数器，最多允许cMaxBackGroundTaskThreads个线程同时执行
	void IncreaseBackGroundWorkingthreads(void) noexcept { m_NumberOfBackGroundWorkingThreads++; }  // 同时运行线程数加一
	void DecreaseBackGroundWorkingthreads(void) noexcept { m_NumberOfBackGroundWorkingThreads--; } // 同时运行线程数减一
	bool IsBackGroundthreadsWorking(void) noexcept { if (m_NumberOfBackGroundWorkingThreads > 0) return true; else return false; } // 计算日线的线程是否处于运行中
	int GetNumberOfBackGroundWorkingThread(void) noexcept { return m_NumberOfBackGroundWorkingThreads; }

	void IncreaseSavingThread(void) noexcept { m_NumberOfSavingThread++; }
	void DecreaseSavingThread(void) noexcept { if (m_NumberOfSavingThread > 0) m_NumberOfSavingThread--; }
	int GetNumberOfSavingThread(void) noexcept { const int iNumber = m_NumberOfSavingThread; return iNumber; }
	bool IsSavingThreadRunning(void) noexcept { if (GetNumberOfSavingThread() > 0) return true; else return false; }

	void IncreaseWebInquiringThread(void) noexcept { m_NumberOfWebInquiringThread++; }
	void DecreaseWebInquiringThread(void) noexcept { if (m_NumberOfWebInquiringThread > 0) m_NumberOfWebInquiringThread--; }
	int GetNumberOfWebInquiringThread(void) noexcept { const int iNumber = m_NumberOfWebInquiringThread; return iNumber; }
	bool IsWebInquiringThreadRunning(void) noexcept { if (GetNumberOfWebInquiringThread() > 0) return true; else return false; }

protected:
	atomic_int m_NumberOfBackGroundWorkingThreads;  // 正在计算日线相对强度的线程数。目前最多同时允许BackGroundThreadPermittedNumber个线程
	atomic_int m_NumberOfSavingThread; // 正在运行的存储工作线程数量
	atomic_int m_NumberOfWebInquiringThread; // 正在运行的提取网络数据工作线程数量
};

extern CThreadStatus gl_ThreadStatus; // 系统中的各种状态，被各个工作线程所使用
