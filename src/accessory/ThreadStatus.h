#pragma once

#include<semaphore>
#include<atomic>
using std::counting_semaphore;
using std::atomic_int;

extern counting_semaphore<4> gl_SaveDayLineThreadPermitted; //Ĭ��Ϊ1.�����ݿ���û����������ʱ�����Ӵ��ź������ֵ��4

class CThreadStatus final {
	//���߳�״̬
public:
	CThreadStatus();
	//ֻ����һ��ʵ��,������ֵ��
	CThreadStatus(const CThreadStatus&) = delete;
	CThreadStatus& operator=(const CThreadStatus&) = delete;
	CThreadStatus(const CThreadStatus&&) noexcept = delete;
	CThreadStatus& operator=(const CThreadStatus&&) noexcept = delete;
	~CThreadStatus() = default;

	//����ִ�м����������ǿ�ȵļ��������������cMaxBackGroundTaskThreads���߳�ͬʱִ��
	void IncreaseBackGroundWorkingThread() noexcept { ++m_NumberOfBackGroundWorkingThreads; } //ͬʱ�����߳�����һ
	void DecreaseBackGroundWorkingThread() noexcept { --m_NumberOfBackGroundWorkingThreads; } //ͬʱ�����߳�����һ
	[[nodiscard]] bool IsBackGroundThreadsWorking() const noexcept { return m_NumberOfBackGroundWorkingThreads > 0; } //�������ߵ��߳��Ƿ���������
	[[nodiscard]] int GetNumberOfBackGroundWorkingThread() const noexcept { return m_NumberOfBackGroundWorkingThreads; }

	void IncreaseSavingThread() noexcept { ++m_NumberOfSavingThread; }
	void DecreaseSavingThread() noexcept { if (m_NumberOfSavingThread > 0) --m_NumberOfSavingThread; }
	[[nodiscard]] int GetNumberOfSavingThread() const noexcept { return m_NumberOfSavingThread; }
	[[nodiscard]] bool IsSavingThreadRunning() const noexcept { return m_NumberOfSavingThread > 0; }

	void IncreaseWebInquiringThread() noexcept { ++m_NumberOfWebInquiringThread; }
	void DecreaseWebInquiringThread() noexcept { if (m_NumberOfWebInquiringThread > 0) --m_NumberOfWebInquiringThread; }
	int GetNumberOfWebInquiringThread() const noexcept { return m_NumberOfWebInquiringThread; }
	[[nodiscard]] bool IsWebInquiringThreadRunning() const noexcept { return m_NumberOfWebInquiringThread > 0; }

protected:
	atomic_int m_NumberOfBackGroundWorkingThreads; //���ڼ����������ǿ�ȵ��߳�����Ŀǰ���ͬʱ����BackGroundThreadPermittedNumber���߳�
	atomic_int m_NumberOfSavingThread; //�������еĴ洢�����߳�����
	atomic_int m_NumberOfWebInquiringThread; //�������е���ȡ�������ݹ����߳�����
};

extern CThreadStatus gl_ThreadStatus; //ϵͳ�еĸ���״̬�������������߳���ʹ��
