#pragma once

#include<semaphore>
#include<atomic>

using std::counting_semaphore;
using std::atomic_int;

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

	[[nodiscard]] static bool IsSavingThreadRunning() noexcept;

	void IncreaseWebInquiringThread() noexcept { ++m_NumberOfWebInquiringThread; }
	void DecreaseWebInquiringThread() noexcept {
		ASSERT(m_NumberOfWebInquiringThread >= 0);
		--m_NumberOfWebInquiringThread;
	}
	int GetNumberOfWebInquiringThread() const noexcept { return m_NumberOfWebInquiringThread; }
	[[nodiscard]] bool IsWebInquiringThreadRunning() const noexcept { return m_NumberOfWebInquiringThread > 0; }

protected:
	atomic_int m_NumberOfBackGroundWorkingThreads; //���ڼ����������ǿ�ȵ��߳�����Ŀǰ���ͬʱ����BackGroundThreadPermittedNumber���߳�
	atomic_int m_NumberOfWebInquiringThread; //�������е���ȡ�������ݹ����߳�����
};

extern CThreadStatus gl_ThreadStatus; //ϵͳ�еĸ���״̬�������������߳���ʹ��
