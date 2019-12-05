#pragma once

#include"afxmt.h"

#include"globedef.h"

using namespace std;
#include<atomic>

const int gl_cMaxCalculatingRSThreads = 80;

class CThreadStatus {    // ���߳�״̬
public:
  CThreadStatus();
  ~CThreadStatus();

  // �߳��˳����״̬�����á��˱�־����ϵͳ�˳��������б���رյ��̹߳رպ󷽿�����ϵͳ�˳�������ϵͳ��Ҫ�ȴ���
  void SetExitingThread(bool fFlag) { m_ExitingThread = fFlag; }
  bool IsExitingThread(void) { return m_ExitingThread; }

  // �����������������ǿ����������
  void SetCalculatingDayLineRS(bool fFlag) { m_CalculateDayLineRelativeStrong = fFlag; }
  bool IsCalculatingDayLineRS(void) { return m_CalculateDayLineRelativeStrong; }

  // ʵʱ������Ҫ������������
  void SetRTDataNeedCalculate(bool fFlag) { m_RTDataNeedCalculate = fFlag; }
  bool IsRTDataNeedCalculate(void) { return m_RTDataNeedCalculate; }

  // ʵʱ���ݼ�������������
  void SetCalculatingRTData(bool fFlag) { m_CalculatingRTData = fFlag; }
  bool IsCalculatingRTData(void) { return m_CalculatingRTData; }

  // ��ʱ���ݴ洢����������
  void SetSavingTempData(bool fFlag) { m_SavingTempData = fFlag; }
  bool IsSavingTempData(void) { return m_SavingTempData; }

  // ��Ʊ����洢����������
  void SetSavingStockCodeData(bool fFlag) { m_SavingStockCodeData = fFlag; }
  bool IsSavingStockCodeData(void) { return m_SavingStockCodeData; }

  // ����ִ�м����������ǿ�ȵļ��������������gl_cMaxCalculatingRSThreads���߳�ͬʱִ��
  void IncreaseNunberOfCalculatingRSThreads(void) { m_CounterOfCalculatingRSThreads++; }  // ͬʱ�����߳�����һ
  void DecreaseNumberOfCalculatingRSThreads(void) { m_CounterOfCalculatingRSThreads--; } // ͬʱ�����߳�����һ
  bool IsCalculatingRS(void) { if (m_CounterOfCalculatingRSThreads > 0) return true; else return false; } // �������ߵ��߳��Ƿ���������
  int HowManyThreadsCalculatingDayLineRS(void) { return m_CounterOfCalculatingRSThreads; }

  void IncreaseNunberOfSavingDayLineThreads(void) { m_SavingDayLine++; }  // ͬʱ�����߳�����һ
  void DecreaseNumberOfSavingDayLineThreads(void) { m_SavingDayLine--; } // ͬʱ�����߳�����һ
  bool IsSavingDayLine(void) { if (m_SavingDayLine > 0) return true; else return false; }  // �������ߵ��߳��Ƿ���������

protected:
  atomic_bool m_ExitingThread;
  atomic_bool m_CalculateDayLineRelativeStrong;
  atomic_bool m_RTDataNeedCalculate;
  atomic_bool m_CalculatingRTData;
  atomic_bool m_SavingTempData;
  atomic_bool m_SavingStockCodeData;

  atomic_int m_SavingDayLine; // �洢������ʷ���ݵļ�����
  atomic_int m_CounterOfCalculatingRSThreads;  // ���ڼ����������ǿ�ȵ��߳�����Ŀǰ���ͬʱ����gl_cMaxCalculatingRSThreads���߳�
};
