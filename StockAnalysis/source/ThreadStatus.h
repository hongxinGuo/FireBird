#pragma once

using namespace std;
#include<atomic>

constexpr int cMaxBackGroundTaskThreads = 8;

class CThreadStatus  final { // ���߳�״̬
public:
	CThreadStatus();
	~CThreadStatus();

	// �����������������ǿ����������
	void SetCalculatingDayLineRS(bool fFlag) noexcept { m_CalculatingDayLineRS = fFlag; }
	bool IsCalculatingDayLineRS(void) noexcept { return m_CalculatingDayLineRS; }

	// �����������������ǿ����������
	void SetCalculatingWeekLineRS(bool fFlag) noexcept { m_CalculatingWeekLineRS = fFlag; }
	bool IsCalculatingWeekLineRS(void) noexcept { return m_CalculatingWeekLineRS; }

	// ʵʱ������Ҫ������������
	void SetRTDataNeedCalculate(bool fFlag) noexcept { m_RTDataNeedCalculate = fFlag; }
	bool IsRTDataNeedCalculate(void) noexcept { return m_RTDataNeedCalculate; }

	// ʵʱ���ݼ�������������
	void SetCalculatingRTData(bool fFlag) noexcept { m_CalculatingRTData = fFlag; }
	bool IsCalculatingRTData(void) noexcept { return m_CalculatingRTData; }

	// ��ʱ���ݴ洢����������
	void SetSavingTempData(bool fFlag) noexcept { m_SavingTempData = fFlag; }
	bool IsSavingTempData(void) noexcept { return m_SavingTempData; }

	// ����������ʷ����
	void SetCreatingWeekLine(bool fFlag) noexcept { m_CreatingWeekLine = fFlag; }
	bool IsCreatingWeekLine(void) noexcept { return m_CreatingWeekLine; }

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
	atomic_bool m_CalculatingDayLineRS;
	atomic_bool m_CalculatingWeekLineRS;
	atomic_bool m_RTDataNeedCalculate;
	atomic_bool m_CalculatingRTData;
	atomic_bool m_SavingTempData;
	atomic_bool m_CreatingWeekLine;

	atomic_int m_NumberOfBackGroundWorkingThreads;  // ���ڼ����������ǿ�ȵ��߳�����Ŀǰ���ͬʱ����cMaxBackGroundTaskThreads���߳�
	atomic_int m_NumberOfSavingThread; // �������еĴ洢�����߳�����
	atomic_int m_NumberOfWebInquiringThread; // �������е���ȡ�������ݹ����߳�����
};
