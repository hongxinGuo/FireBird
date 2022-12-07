#pragma once

using namespace std;
#include<atomic>
#include<semaphore>

extern counting_semaphore<4> gl_SaveDayLineThreadPermitted; // Ĭ��Ϊ1.�����ݿ���û����������ʱ�����Ӵ��ź������ֵ��4

class CThreadStatus  final { // ���߳�״̬
public:
	CThreadStatus();
	// ֻ����һ��ʵ��,������ֵ��
	CThreadStatus(const CThreadStatus&) = delete;
	CThreadStatus& operator=(const CThreadStatus&) = delete;
	CThreadStatus(const CThreadStatus&&) noexcept = delete;
	CThreadStatus& operator=(const CThreadStatus&&) noexcept = delete;
	~CThreadStatus();

	// ����ִ�м����������ǿ�ȵļ��������������cMaxBackGroundTaskThreads���߳�ͬʱִ��
	void IncreaseBackGroundWorkingthreads(void) noexcept { m_NumberOfBackGroundWorkingThreads++; }  // ͬʱ�����߳�����һ
	void DecreaseBackGroundWorkingthreads(void) noexcept { m_NumberOfBackGroundWorkingThreads--; } // ͬʱ�����߳�����һ
	bool IsBackGroundthreadsWorking(void) noexcept { if (m_NumberOfBackGroundWorkingThreads > 0) return true; else return false; } // �������ߵ��߳��Ƿ���������
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
	atomic_int m_NumberOfBackGroundWorkingThreads;  // ���ڼ����������ǿ�ȵ��߳�����Ŀǰ���ͬʱ����BackGroundThreadPermittedNumber���߳�
	atomic_int m_NumberOfSavingThread; // �������еĴ洢�����߳�����
	atomic_int m_NumberOfWebInquiringThread; // �������е���ȡ�������ݹ����߳�����
};

extern CThreadStatus gl_ThreadStatus; // ϵͳ�еĸ���״̬�������������߳���ʹ��
